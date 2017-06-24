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

SFUint32 nOutOfBal=0;
//-----------------------------------------------------------------------
SFString nameAccount(const SFString& address) {
    SFString name = address;
         if (address % "0xbb9bc244d798123fde783fcc1c72d3bb8c189413") name = "The DAO";
    else if (address % "0x807640a13483f8ac783c557fcdf27be11ea4ac7a") name = "Extra Balance";
    if (name.length() >= 42)
        return name;
    size_t need = 42 - name.length() - 6;  // '6' for ' (' and '...)'
    name = name + " (" + address.substr(0,need) + "...)";
    return name;
}

//-----------------------------------------------------------------------
void showColoredBloom(const SFBloom& bloom, const SFString& msg, const SFString& res) {
    SFString bl = fromBloom(bloom).substr(2);
    for (int i = 0 ; i < bl.length() ; i = i + 128) {
        SFString m = padLeft(" ",16);
        if (i == 0)
            m = "    " + msg + bBlack + " 0x";
        cout << m << bl.substr(i,128).Substitute("0",bBlack+"."+cOff) << (i == 0 ? " "+res : "") << "\r\n";
    }
}

//-----------------------------------------------------------------------
void CVisitor::showColoredTrace(const SFHash& hash, bool err) {

    CTraceArray traces;
    getTraces(traces, hash);

    for (int t = 0 ; t < traces.getCount() ; t++) {
        const CTrace *tt = &traces[t];
        if (t > 0) {
            CBlock unused;
            accountForIntTransaction(unused, tt);
        }
        SFString from = tt->Format("[{ACTION::FROM}]");
        SFString to = tt->Format("[{ACTION::TO}]");
        SFString value = tt->Format("[{ACTION::VALUE}]");
        SFString subtraces = tt->Format("[{SUBTRACES}]");
        SFString traceAddress = tt->Format("[{TRACEADDRESS}]");
        if (traceAddress.empty())
            traceAddress = "[ ]";

        for (int i = 0 ; i < watches.getCount()-1 ; i++) {
            SFString c1 = watches[i].color, c2 = cOff;
            if (err) { c1 = c2 = biBlack; }
            from.Replace(watches[i].address, c1 + nameAccount(watches[i].address) + c2);
              to.Replace(watches[i].address, c1 + nameAccount(watches[i].address) + c2);
        }

        {
            SFString exBal = "0x807640a13483f8ac783c557fcdf27be11ea4ac7a";
            SFString c1 = bBlue, c2 = cOff;
            if (err) { c1 = c2 = biBlack; }
            from.Replace(exBal, c1 + nameAccount(exBal) + c2);
            to.Replace(exBal, c1 + nameAccount(exBal) + c2);
        }

        SFString c1 = biBlack, c2 = cOff, c3 = bRed+italic;
        if (err) { c1 = c2 = c3 = biBlack; }
        if (from.length()) {
            cout << c1 <<  "\r\n    {"           << c2;
            cout << c1 <<  " \"from\": "         << c2 << from;
            cout << c1 << ", \"to\": "           << c2 << to;
            cout << c1 << ", \"subtraces\": "    << c2 << subtraces;
            cout << c1 << ", \"traceAddress\": " << c2 << traceAddress;

            SFUintBN wei = canonicalWei(value);
            cout << c1 << ", \"value\": ";
            cout << (wei == 0 ? "" : c3);
            cout << wei2Ether(asStringBN(wei)) << c2;
            cout << c1 << " }" << cOff;
        }
    }
    cout << "\r\n";
}

//-----------------------------------------------------------------------
void CVisitor::openIncomeStatement(const CBlock& block) {
    if (!accounting_on)
        return;
    for (int i = 0 ; i < watches.getCount() ; i++) {
        CAccountWatch *w = &watches[i];
        w->qbis.inflow = w->qbis.outflow = w->qbis.gasCost = 0;
        if (i < watches.getCount()-1) {
            SFIntBN curBal = getBalance(w->address, block.blockNumber-1, false);
            if (curBal != w->qbis.endBal) {
                cout << bRed
                << w->address << " is out of balance ("
                << curBal << " - " << w->qbis.endBal
                << ") between end of block "
                << 'x' << " and block "
                << block.blockNumber << "\r\n" << cOff;
//                if (debugger_on) {
//                    enterDebugger(block);
//                }
            }
        }
    }
    if (nAccountedFor) // depends on previous value. Bad.
        cout << "\r\n" << bYellow << SFString('-', 180) << "\r\n";
    nAccountedFor = 0;
    nOutOfBal = 0;
    return;
}

//-----------------------------------------------------------------------
void CVisitor::accountForExtTransaction(const CBlock& block, const CTransaction *trans) {

    if (!accounting_on)
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

    // Nothing to record if there was an error, but we do have to account for gas
    if (trans->isError) { // || trans->value == 0)
        if (fWhich != watches.getCount() - 1)
            watches[fWhich].qbis.gasCost += (trans->receipt.gasUsed * trans->gasPrice);
        return;
    }

    watches[fWhich].qbis.outflow += trans->value;
    if (fWhich != watches.getCount() - 1)
        watches[fWhich].qbis.gasCost += (trans->receipt.gasUsed * trans->gasPrice);
    watches[tWhich].qbis.inflow += trans->value;
    nAccountedFor++;
    tBuffer.addItem(trans->blockNumber, trans->transactionIndex, trans->hash);
}

//-----------------------------------------------------------------------
void CVisitor::accountForIntTransaction(const CBlock& block, const CTrace *trace) {

    if (!accounting_on)
        return;

    // find the contracts we have to account for...
    uint32_t tWhich = watches.getCount() - 1;
    uint32_t fWhich = watches.getCount() - 1;
    for (int i = 0 ; i < watches.getCount() - 1 ; i++) {
        if (trace->action.to.ContainsI(watches[i].address))
            tWhich = i;
        if (trace->action.from.ContainsI(watches[i].address))
            fWhich = i;
    }

#if 0
    // Traces never happen with in-error transactions, so this i redundant, we think
    if (trans->isError) // || trans->value == 0)
        return;
#endif

    watches[fWhich].qbis.outflow += trace->action.value;
//    if (fWhich != watches.getCount() - 1)
//        watches[fWhich].qbis.gasCost += (trans->receipt.gasUsed * trace->gasPrice);
    watches[tWhich].qbis.inflow += trace->action.value;
    nAccountedFor++;
//    tBuffer.addItem(trace->blockNumber, trace->transactionIndex, trace->hash);
}

//-----------------------------------------------------------------------
bool CVisitor::closeIncomeStatement(const CBlock& block) {
    // TODO(tjayrush): when should the account display show itself?
    if (!accounting_on || !nProcessed())  // don't report until something interesting happened
        return true;

    CIncomeStatement header;
    header.begBal = header.endBal = -1;

    cout << padCenter("",14) << header << "   " << padCenter("nodeBal",28) << "\r\n";
    CIncomeStatement total;
    for (int i = 0 ; i < watches.getCount() ; i++) {
        watches[i].qbis.blockNum = block.blockNumber;
        watches[i].qbis.begBal = watches[i].qbis.endBal;
        watches[i].qbis.endBal = (watches[i].qbis.begBal + watches[i].qbis.inflow - watches[i].qbis.outflow - watches[i].qbis.gasCost);
        cout << watches[i].color << padRight(watches[i].name,14) << cOff << watches[i].qbis << "   ";
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
        cout << "\r\n";
        total += watches[i].qbis;
    }
    cout << "              " << SFString('-',120) << "\r\n";
    cout << padRight("Total:",14) << total << " ";
    cout << greenCheck;
    cout << "\r\n" << cOff;
    cout.flush();

    if (debugger_on) {
        if (nOutOfBal || single_on) {
            if (!enterDebugger(block))
                return false;  // quit
        }
    } else if (single_on) {
        cout << "\r\nHit enter to continue or 'q' to quit >> ";
        char ch = getchar();
        if (ch == 'q')
            return false;
    }

    // TODO(tjayrush): when should auto correct be on or off?
    if (autoCorrect) {
        for (int i = 0 ; i < watches.getCount() ; i++)
            watches[i].qbis.correct();
    }
    return true;
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
                        displayTransaction(which, trans, visitor);
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
                displayTransaction(which, trans, visitor);
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
