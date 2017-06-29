/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
/*
 * This code was generated automatically from grabABI and makeClass. You may
 * edit the file, but keep your edits inside the 'EXISTING_CODE' tags.
 */
#include "parselib.h"
#include "debug.h"

//-----------------------------------------------------------------------
bool CVisitor::ofInterest(CTransaction *trans, uint32_t& which) {

    // Assume it's not an internal transaction
    trans->isInternalTx = false;

    // First, check to see if the transaction is directly 'to' or 'from'
    for (int i = 0 ; i < watches.getCount()-1 ; i++) {
        if (trans->to.ContainsI(watches[i].address) || trans->from.ContainsI(watches[i].address)) {
            which = i;
            return true;
        }

        // If this is a contract and this is its birth block, that's a hit
        if (trans->receipt.contractAddress == watches[i].address) {
            which = i;
            trans->isInternalTx = true;  // TODO(tjayrush) - handle contract creation correctly (change to data)
            return true;
        }
    }

    // Next, we check the receipt logs to see if the address appears either in
    // the log's 'address' field or in one of the data items
    //
    // TODO(tjayrush): We should do a 'deep trace' here (or when the block is first read)
    // to see if there was a 'call,' to our address.
    for (int i = 0 ; i < trans->receipt.logs.getCount() ; i++) {
        for (int k = 0 ; k < watches.getCount()-1 ; k++) {
            SFString acc = watches[k].address;

            CLogEntry *l = reinterpret_cast<CLogEntry *>(&trans->receipt.logs[i]);
            if (l->address.Contains(acc)) {
                // If we find a receipt log with an 'address' of interest, this is an
                // internal transaction that caused our contract to emit an event.
                which = k;
                trans->isInternalTx = true;
                return true;

            } else {
                // Next, left pad the address with '0' to a width of 32 bytes. If
                // it matches either an indexed topic or one of the 32-byte aligned
                // data items, we have found a potential match. We cannot be sure this
                // is a hit, but it most likely is. This may be a false positive.
                acc = padLeft(acc.Substitute("0x",""), 64).Substitute(' ', '0');
                if (l->data.ContainsI(acc)) {
                    // Do this first to avoid spinning through event sigs if we
                    // don't have to.
                    which = k;
                    trans->isInternalTx = true;
                    return true;

                } else {
                    // If the topic[0] is an event of interest...
                    for (int q = 0 ; q < nSigs ; q++) {
                        SFHash tHash = fromTopic(l->topics[0]);
                        if (tHash % sigs[q]) {
                            which = k;
                            trans->isInternalTx = true;
                            return true;
                        }
                    }

                    // ...or the address is in the indexed topics or data
                    for (int j = 1 ; j < l->topics.getCount() ; j++) {
                        SFHash tHash = fromTopic(l->topics[j]);
                        if (tHash % acc) {
                            which = k;
                            trans->isInternalTx = true;
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

//-----------------------------------------------------------------------
bool displayFromCache(const SFString& cacheFileName, SFUint32& blockNum, void *data) {

    CVisitor *visitor = reinterpret_cast<CVisitor*>(data);
    CBlock block;

    // If there is no cache, there's nothing to display
    if (!fileExists(cacheFileName))
        return true; // return true if we want to continue on to updateCache

    SFUint32 orig = blockNum, lastBlock = 0;

    if (visitor->cache.Lock(cacheFileName, binaryReadOnly, LOCK_NOWAIT)) {

        while (!visitor->cache.Eof()) {

            SFUint32 transID;
            uint32_t which;
            visitor->cache >> which;
            visitor->cache >> blockNum;
            visitor->cache >> transID;

            if (blockNum >= orig) {

                if (blockNum > lastBlock) {  // only re-read if it's a new block

                    if (!visitor->closeIncomeStatement(block)) {
                        cerr << "Quitting debugger.\r\n";
                        visitor->cache.Release();
                        return false; // return false since user hit 'quit' on debugger
                    }

                    // If we switched blocks, read the next block
                    visitor->lastBlockNum = block.blockNumber;
                    if (!readOneBlock_fromBinary(block, getBinaryFilename1(blockNum))) {
                        cerr << "Block read failed. Quitting cache read\r\n";
                        visitor->cache.Release();
                        return false;
                    }
                    visitor->openIncomeStatement(block);
                    lastBlock = blockNum;
                    if (verbose)
                        visitor->interumReport1(block.blockNumber, block.timestamp);
                }

                // this will always be true, but we just protect ourselves here
                if (transID < block.transactions.getCount()) {
                    CTransaction *trans = &block.transactions[(uint32_t)transID];
                    trans->pBlock = &block;
                    if (!visitor->watches[which].disabled) {
                        if (visitor->debugger_on)
                            cout << bGreen << SFString('-',180) << "\r\n";
                        visitor->displayTransaction(which, trans, visitor);
                        visitor->accountForExtTransaction(block, trans);
                    }
                }
            }
        }
        // ignore return since we're done anway
        visitor->closeIncomeStatement(block);
        visitor->cache.Release();
    }
    return true;
}

//-----------------------------------------------------------------------
bool updateCache(CBlock& block, void *data) {

    CVisitor *visitor = reinterpret_cast<CVisitor*>(data);

    visitor->openIncomeStatement(block);
    for (int i = 0 ; i < block.transactions.getCount() ; i++) {

        CTransaction *trans = &block.transactions[i];
        trans->pBlock = &block;

        uint32_t which;
        if (visitor->ofInterest(trans, which)) {

            // Display only if the user is interested in this account
            if (!visitor->watches[which].disabled) {
                visitor->displayTransaction(which, trans, visitor);
                visitor->accountForExtTransaction(block, trans);
            }

            ASSERT(!visitor->cache.m_isReading);
            // Write the data even if we're not displaying it (flush to make sure it gets written)
            visitor->cache << which << trans->pBlock->blockNumber << trans->transactionIndex;
            visitor->cache.flush();
            visitor->nFreshened++;
        }
    }

    if (verbose) {
        timestamp_t tsOut = (block.timestamp == 0 ? toTimeStamp(Now()) : block.timestamp);
        SFString endMsg = dateFromTimeStamp(tsOut).Format(FMT_JSON) + " (" + asString(block.blockNumber) + ")";
        visitor->interumReport(block.blockNumber, block.timestamp, endMsg);
    }

    // Write this to the file so we know which block to start on next time the monitor is run
    stringToAsciiFile("./cache/lastBlock.txt", asStringU(block.blockNumber) + "\r\n");
    return visitor->closeIncomeStatement(block);  // may invoke debugger, which may return false, which will stop update
}
