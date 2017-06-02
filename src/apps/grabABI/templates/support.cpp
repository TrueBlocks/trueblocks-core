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
    }

    // Next, we check the receipt logs to see if the address appears either in
    // the log's 'address' field or in one of the data items
    //
    // TODO(tjayrush): We should do a 'deep trace' here (or when the block is first read)
    // to see if there was a 'call,' to our address. Or, better, use logsBloom
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
                acc = padLeft(acc, 64).Substitute(' ', '0');
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

SFUint32 nOutOfBal=0;
//-----------------------------------------------------------------------
void CVisitor::openIncomeStatement(const CBlock& block) {
    if (!accounting)
        return;
    for (int i = 0 ; i < watches.getCount() ; i++) {
        watches[i].qbis.inflow = watches[i].qbis.outflow = 0;
    }
    if (nTrans)
        cout << SFString('=',150) << "\n";
    nTrans = 0;
    nOutOfBal = 0;
    return;
}

//-----------------------------------------------------------------------
void showColoredTrace(CVisitor *vis, const SFHash& hash) {

    SFString traceStrIn;
    queryRawTrace(traceStrIn, hash, true);

//#define PARSETRACE
#ifdef PARSETRACE
    SFString traceStr;
    char *p = cleanUpJson((char *)(traceStrIn.c_str()));  // NOLINT
    while (p && *p) {
        CTrace trace;
        uint32_t nFields = 0;
        p = trace.parseJson(p, nFields);
        if (nFields) {
            traceStr += (trace.Format() + "\n");
        }
    }
#else
    SFString traceStr = traceStrIn;
#endif

    for (int i = 0 ; i < vis->watches.getCount()-1 ; i++) { // don't show extra accounts
        traceStr.ReplaceAll(vis->watches[i].address,vis->watches[i].color+vis->watches[i].name+" ("+vis->watches[i].address+") "+cOff);
    }
    while (traceStr.Contains("\"value\":\""))
        traceStr.ReplaceAll("\"value\":\"", "|");
    SFString str;
    while (!traceStr.empty())
    {
        SFString front = nextTokenClear(traceStr, '|');
        SFString val = nextTokenClear(traceStr, '\"');
        str += (front);
        if (!val.empty())
            str += (SFString("\"value\":\"") + bRed + asStringBN(canonicalWei(val)) + "\"" + cOff);
    }
    cout << "\n" << cOff << str;
}

//-----------------------------------------------------------------------
bool CVisitor::closeIncomeStatement(const CBlock& block) {
    if (!accounting || nTrans == 0)
        return true;

    CIncomeStatement header;
    header.begBal = header.endBal = -1;

    cout << padCenter("",14) << header << "   " << padCenter("nodeBal",28) << "\n";
    CIncomeStatement total;
    for (int i = 0 ; i < watches.getCount() ; i++) {
        watches[i].qbis.blockNum = block.blockNumber;
        watches[i].qbis.begBal = watches[i].qbis.endBal;
        watches[i].nodeis.begBal = watches[i].nodeis.endBal;
        watches[i].qbis.endBal = (watches[i].qbis.begBal + watches[i].qbis.inflow - watches[i].qbis.outflow);
        cout << padRight(watches[i].name,14) << watches[i].qbis << "   ";
        if (i < watches.getCount()-1) {
            watches[i].qbis.reconcile(watches[i].address, block.blockNumber);
            cout << padLeft(wei2Ether(to_string(watches[i].qbis.nodeBal).c_str()),28);
            if (!watches[i].qbis.balanced()) {

                cout << " " << bRed << padLeft(wei2Ether(to_string(watches[i].qbis.difference()).c_str()),28) << cOff << redX;
                nOutOfBal++;
            } else {
                cout << " " << greenCheck;
            }
        }
        cout << "\n";
        total += watches[i].qbis;
    }
    cout << "              " << SFString('-',120) << "\n";
//    cout << cGreen;
    cout << padRight("Total:",14) << total << " ";
    cout << greenCheck;
    cout << "\n" << cOff;
    cout.flush();

    if (debug) {
        if (nOutOfBal || breakPt == UINT64_MAX || block.blockNumber >= breakPt) {
            bool done=false;
            while (!done)
            {
                cerr << cOff << "\n===>  ";
                cerr.flush();
                char buffer[256];
                fgets(buffer, 256, stdin);
                SFString res = Strip(SFString(buffer).Substitute("\n",""),' ');

                if (res == "q" || res == "quit" || res == "exit") {
                    return false;

                } if (res == "c" || res == "correct") {
                    for (int i = 0 ; i < watches.getCount() ; i++) {
                        watches[i].qbis.correct();
                    }
                    done = true;

                } else if (res.startsWith("t:") && res.Contains(".")) {
                    res.Replace("t:","");
                    SFUint32 bn = toLongU(nextTokenClear(res,'.'));
                    SFUint32 tn = toLongU(res);
                    cout << bn << "." << tn << "\n";
                    CTransaction trans;
                    getTransaction(trans,bn,tn);
                    showColoredTrace(this, trans.hash);

                } else if (res == "h" || res == "help") {
                    cout << "q = quit, c = correcting entry, t:bn.tn = trace at blockNum.transID, s:N = step N steps, c = clear, h = help, enter to continue\n";

                } else if (!res.empty()) {
                    cout << "invalid command: " << res << "\n";
                    
                } else {
                    done = true;
                }
            }
        }
    }
    return true;
}

//-----------------------------------------------------------------------
void CVisitor::accountForTransaction(const CTransaction *trans) {

    if (!accounting)
        return;

    // Nothing to record if there was an error or no value
    if (trans->isError || trans->value == 0)
        return;

    // find the contracts we have to account for...
    uint32_t tWhich = watches.getCount() - 1;
    uint32_t fWhich = watches.getCount() - 1;
    for (int i = 0 ; i < watches.getCount() - 1 ; i++) {
        if (trans->to.ContainsI(watches[i].address))
            tWhich = i;
        if (trans->from.ContainsI(watches[i].address))
            fWhich = i;
    }

    watches[fWhich].qbis.outflow += trans->value;
    watches[tWhich].qbis.inflow += trans->value;
    nTrans++;
}

//-----------------------------------------------------------------------
int displayCache(const SFString& cacheFileName, SFUint32& blockNum, void *data) {

    // If there is no cache, there's nothing to display
    if (!fileExists(cacheFileName))
        return false;

    CBlock block;
    SFUint32 orig = blockNum, lastBlock = 0;
    CVisitor *visitor = reinterpret_cast<CVisitor*>(data);

    if (visitor->cache.Lock(cacheFileName, binaryReadOnly, LOCK_NOWAIT)) {

        while (!visitor->cache.Eof()) {

            SFUint32 transID;
            uint32_t which;
            visitor->cache >> which;
            visitor->cache >> blockNum;
            visitor->cache >> transID;

            if (blockNum >= orig) {

                if (blockNum > lastBlock) {  // only re-read if it's a new block

                    // If we switched blocks, read the next block
                    if (!readOneBlock_fromBinary(block, getBinaryFilename1(blockNum))) {
                        cerr << "Block read failed. Quitting cache read\n";
                        visitor->cache.Release();
                        return false;
                    }
                    bool cont = visitor->closeIncomeStatement(block);
                    if (!cont)
                        return false;
                    visitor->openIncomeStatement(block);
                    lastBlock = blockNum;
                    if (verbose)
                        visitor->stats.interumReport1(block.blockNumber, block.timestamp);
                }

                // this will always be true, but we just protect ourselves here
                if (transID < block.transactions.getCount()) {
                    CTransaction *trans = &block.transactions[(uint32_t)transID];
                    trans->pBlock = &block;
                    if (!visitor->watches[which].disabled) {
                        displayTransaction(which, trans, visitor);
                        visitor->accountForTransaction(trans);
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
bool buildCache(CBlock& block, void *data) {

    CVisitor *visitor = reinterpret_cast<CVisitor*>(data);

    visitor->openIncomeStatement(block);
    for (int i = 0 ; i < block.transactions.getCount() ; i++) {

        CTransaction *trans = &block.transactions[i];
        trans->pBlock = &block;

        uint32_t which;
        if (visitor->ofInterest(trans, which)) {

            // Display only if the user is interested in this account
            if (!visitor->watches[which].disabled) {
                displayTransaction(which, trans, visitor);
                visitor->accountForTransaction(trans);
            }

            ASSERT(!visitor->cache.m_isReading);
            // Write the data even if we're not displaying it (flush to make sure it gets written)
            visitor->cache << which << trans->pBlock->blockNumber << trans->transactionIndex;
            visitor->cache.flush();
        }

        if (verbose)
            visitor->stats.interumReport(block.blockNumber, block.timestamp);
    }
    // Write this to the file so we know which block to start on next time the monitor is run
    stringToAsciiFile("./cache/lastBlock.txt", asStringU(block.blockNumber) + "\n");
    return visitor->closeIncomeStatement(block);
}
