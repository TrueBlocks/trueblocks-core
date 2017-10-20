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
bool CVisitor::openIncomeStatement(const CBlock& block) {
    if (!opts.accounting_on)
        return true;

    for (uint32_t i = 0 ; i < watches.getCount() ; i++) {
        CAccountWatch *w = &watches[i];
        w->qbis.inflow = w->qbis.outflow = w->qbis.gasCost = 0;
        if (i < watches.getCount()-1) {
            w->qbis.nodeBal = getBalance(w->address, block.blockNumber-1, false);
            SFIntBN diff = (w->qbis.nodeBal - w->qbis.endBal);
            if (diff != 0) {
                SFString c1 = watches[i].color, c2 = cOff;
                cout << "\r\n" << bRed << SFString('-', 5) << " WARNING " << SFString('-', 166) << "\r\n";
                cout
                    << c1 << w->address << c2 << " is out of balance by "
                    << cRed << wei2Ether(asStringBN(diff)) << cOff
                    << " ether at the start of block "
                    << cYellow << block.blockNumber << "\r\n" << cOff;
                cout << bRed << SFString('-', 180) << cOff << "\r\n";
                if (opts.debugger_on) {
                    if (!enterDebugger(block))
                            return false;  // quit
                }
            }
        }
    }
    transStats.nAccountedFor = 0;
    return true;
}

//-----------------------------------------------------------------------
bool CVisitor::accountForExtTransaction(const CBlock& block, const CTransaction *trans) {

    if (!opts.accounting_on)
        return true;

    for (uint32_t t = 0 ; t < trans->traces.getCount() ; t++) {
        const CTrace *tt = &trans->traces[t];
        if (t > 0) {
            CBlock unused;
            if (!trans->isError) {
                accountForIntTransaction(unused, trans, tt);
            }
        }
    }

    // find the contracts we have to account for...
    uint32_t tWhich = watches.getCount() - 1;
    uint32_t fWhich = watches.getCount() - 1;
    for (uint32_t i = 0 ; i < watches.getCount() - 1 ; i++) {
        if (trans->to.ContainsI(watches[i].address))
            tWhich = i;
        if (trans->from.ContainsI(watches[i].address))
            fWhich = i;
    }

    // Nothing to record if there was an error, but we do have to account for gas
    if (trans->isError) { // || trans->value == 0)
        if (fWhich != watches.getCount() - 1) {
            watches[fWhich].qbis.gasCost += (trans->receipt.gasUsed * trans->gasPrice);
            transStats.nAccountedFor++;
        }
        return true;
    }

    watches[fWhich].qbis.outflow += trans->value;
    if (fWhich != watches.getCount() - 1)
        watches[fWhich].qbis.gasCost += (trans->receipt.gasUsed * trans->gasPrice);
    watches[tWhich].qbis.inflow += trans->value;
    transStats.nAccountedFor++;
    tBuffer.addItem(trans->blockNumber, trans->transactionIndex, trans->hash);
    return true;
}

//-----------------------------------------------------------------------
bool CVisitor::accountForIntTransaction(const CBlock& block, const CTransaction *trans, const CTrace *trace) {

    if (!opts.accounting_on)
        return true;

    if (trace->isError())
        return true;

    if (trace->type % "suicide") {
        CTraceAction *action = (CTraceAction*)&(trace->action);
        action->to = action->refundAddress;
        action->from = action->address;
        action->value = action->balance;
    }

    // find the contracts we have to account for...
    uint32_t tWhich = watches.getCount() - 1;
    uint32_t fWhich = watches.getCount() - 1;
    for (uint32_t i = 0 ; i < watches.getCount() - 1 ; i++) {
        if (trace->action.to.ContainsI(watches[i].address))
            tWhich = i;
        if (trace->action.from.ContainsI(watches[i].address))
            fWhich = i;
    }

    watches[fWhich].qbis.outflow += trace->action.value;
//    if (fWhich != watches.getCount() - 1)
//        watches[fWhich].qbis.gasCost += (trans->receipt.gasUsed * trace->gasPrice);
    watches[tWhich].qbis.inflow += trace->action.value;
    transStats.nAccountedFor++;
//    tBuffer.addItem(trace->blockNumber, trace->transactionIndex, trace->hash);
    return true;
}

//-----------------------------------------------------------------------
bool CVisitor::closeIncomeStatement(const CBlock& block) {

    // If the user has not hit the escape key...
    if (!esc_hit) {
        // ...and is either not accounting or nothing worth reporting happened...
        if (!opts.accounting_on || !transStats.nAccountedFor) {
            // ...return
            return true;
        }
    }

    CIncomeStatement total;
    for (uint32_t i = 0 ; i < watches.getCount() ; i++) {
        total += watches[i].qbis;
    }

    uint32_t nOutOfBal = 0;
    if (total.inflow > 0 || (total.outflow + total.gasCost) > 0) {
        CIncomeStatement header;
        header.begBal = header.endBal = -1;

        cout << padCenter("",24) << header << "   " << padCenter("nodeBal",38) << "\r\n";
        cout << bBlack << SFString('-',180) << "\r\n";
        for (uint32_t i = 0 ; i < watches.getCount() ; i++) {
            watches[i].qbis.blockNum = block.blockNumber;
            watches[i].qbis.begBal = watches[i].qbis.endBal;
            watches[i].qbis.endBal = (watches[i].qbis.begBal + watches[i].qbis.inflow - watches[i].qbis.outflow - watches[i].qbis.gasCost);

            cout << watches[i].color << padRight(watches[i].displayName(false,14,6),24) << cOff << watches[i].qbis << "   ";

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
        }
        cout << SFString(' ',23) << SFString('-',125) << "\r\n";
        cout << padRight("Total:",24) << total << " ";
        cout << greenCheck;
        cout << "\r\n" << cOff;
        cout.flush();
        if (transStats.nAccountedFor)
            cout << "\r\n" << bYellow << SFString('-', 180) << "\r\n";
    }

    if (opts.debugger_on) {
        if (nOutOfBal || opts.single_on || debugFile() || esc_hit) {
            if (!enterDebugger(block)) {
                esc_hit = false;
                return false;  // quit
            }
        }
    } else if (opts.single_on) {
        if (!opts.autocorrect_on)
            cout << "\r\nHit enter to continue, 'c' to correct and continue, or 'q' to quit >> ";
        else
            cout << "\r\nHit enter to continue or 'q' to quit >> ";
        int ch = getchar();
        if (ch == 'q') {
            esc_hit = false;
            return false;
        } else if (ch == 'c') {
            for (uint32_t i = 0 ; i < watches.getCount() ; i++)
                watches[i].qbis.correct();
        }
    }

    if (opts.autocorrect_on) {
        for (uint32_t i = 0 ; i < watches.getCount() ; i++)
            watches[i].qbis.correct();
    }
    esc_hit = false;
    return true;
}
