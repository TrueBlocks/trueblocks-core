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
#include "main.h"

//-----------------------------------------------------------------------
bool COptions::openIncomeStatement(const CBlock& block) {
    if (!accounting_on)
        return true;

    for (size_t i = 0 ; i < watches.size() ; i++) {
        CAccountWatch *w = &watches.at(i);
        w->statment.inflow = w->statment.outflow = w->statment.gasCostInWei = 0;
        if (i < watches.size()-1) {
            w->statment.nodeBal = getNodeBal(w->balanceHistory, w->address, block.blockNumber-1);
            bigint_t diff = (w->statment.nodeBal - w->statment.endBal);
            if (!no_check && (diff != 0)) {
                single_on = true;
                string_q c1 = watches[i].color, c2 = cOff;
                cout << "\r\n" << bRed << string_q(5, '-') << " WARNING " << string_q(166, '-') << "\r\n";
                cout
                    << c1 << w->address << c2 << " is out of balance by "
                    << cRed << wei_2_Ether(bni_2_Str(diff)) << cOff
                    << " ether at the start of block "
                    << cYellow << block.blockNumber << "\r\n" << cOff;
                cout << bRed << string_q(180, '-') << cOff << "\r\n";
                if (debugger_on) {
                    if (!enterDebugger(block))
                            return false;  // quit
                }
            }
        }
    }
    transStats.nAccountedFor = 0;
    return true;
}

// THIS IS AN IMPORTANT NOTE - DON'T IGNORE - WE NEED TO ACCOUNT FOR 
// TRACES NOT TRANSACTIONS. SEE ISSUE #738
//-----------------------------------------------------------------------
bool COptions::accountForExtTransaction(const CBlock& block, const CTransaction *trans) {

    if (!accounting_on)
        return true;

    for (size_t t = 0 ; t < trans->traces.size() ; t++) {
        const CTrace *tt = &trans->traces[t];
        if (t > 0) {
            CBlock unused;
            if (!trans->isError) {
                accountForIntTransaction(unused, trans, tt);
            }
        }
    }

    // find the contracts we have to account for...
    size_t tWhich = watches.size() - 1;
    size_t fWhich = watches.size() - 1;
    for (size_t i = 0 ; i < watches.size() - 1 ; i++) {
        if (containsI(trans->to, watches[i].address))
            tWhich = i;
        if (containsI(trans->from, watches[i].address))
            fWhich = i;
    }

    // Nothing to record if there was an error, but we do have to account for gas
    if (trans->isError) { // || trans->value == 0)
        if (fWhich != watches.size() - 1) {
            watches.at(fWhich).statment.gasCostInWei += (trans->receipt.gasUsed * trans->gasPrice);
            transStats.nAccountedFor++;
        }
#ifdef DEBUGGER_ON
        tBuffer.addItem(trans->blockNumber, trans->transactionIndex, trans->hash);
#endif
        return true;
    }

    watches.at(fWhich).statment.outflow += trans->value;
    if (fWhich != watches.size() - 1)
        watches.at(fWhich).statment.gasCostInWei += (trans->receipt.gasUsed * trans->gasPrice);
    watches.at(tWhich).statment.inflow += trans->value;
    transStats.nAccountedFor++;
#ifdef DEBUGGER_ON
    tBuffer.addItem(trans->blockNumber, trans->transactionIndex, trans->hash);
#endif
    return true;
}

//-----------------------------------------------------------------------
bool COptions::accountForIntTransaction(const CBlock& block, const CTransaction *trans, const CTrace *trace) {

    if (!accounting_on)
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
    size_t tWhich = watches.size() - 1;
    size_t fWhich = watches.size() - 1;
    for (size_t i = 0 ; i < watches.size() - 1 ; i++) {
        if (containsI(trace->action.to, watches[i].address))
            tWhich = i;
        if (containsI(trace->action.from, watches[i].address))
            fWhich = i;
    }

    watches.at(fWhich).statment.outflow += trace->action.value;
    // gas is paid by originating account
    //    if (fWhich != watches.size() - 1)
    //        watches[fWhich].statment.gasCostInWei += (trans->receipt.gasUsed * trace->gasPrice);
    watches.at(tWhich).statment.inflow += trace->action.value;
    transStats.nAccountedFor++;
    //    tBuffer.addItem(trace->blockNumber, trace->transactionIndex, trace->hash);
    return true;
}

//-----------------------------------------------------------------------
bool COptions::closeIncomeStatement(const CBlock& block) {

    // If the user has not hit the escape key...
    if (!esc_hit) {
        // ...and is either not accounting or nothing worth reporting happened...
        if (!accounting_on || !transStats.nAccountedFor) {
            // ...return
            return true;
        }
    }

    CIncomeStatement total;
    for (size_t i = 0 ; i < watches.size() ; i++) {
        total += watches[i].statment;
    }

    size_t nOutOfBal = 0;
//TODO(tjayrush) This should be an option
    if (true) { //total.inflow > 0 || (total.outflow + total.gasCostInWei) > 0) {
        CIncomeStatement header;
        header.begBal = header.endBal = -1;

        cout << cOff << string_q(23, ' ') << cWhite << " Profit and Loss for block #" << block.blockNumber << " (" << ts_2_Date(block.timestamp).Format(FMT_JSON) << ")\r\n";
//        cout << bBlack << string_q(23, ' ') << BG << string_q(155, '-') << cOff << "\r\n";
        cout           << padCenter("",23) << " " << BG << header << "  " << padCenter("nodeBal",34) << cOff << "\r\n";
//        cout << bBlack << string_q(23, ' ') << string_q(155, '-') << cOff << "\r\n";
        for (size_t i = 0 ; i < watches.size() ; i++) {
            watches.at(i).statment.blockNum = block.blockNumber;
            watches.at(i).statment.begBal = watches[i].statment.endBal;
            watches.at(i).statment.endBal = (watches[i].statment.begBal + watches[i].statment.inflow - watches[i].statment.outflow - watches[i].statment.gasCostInWei);

            cout << watches[i].color << padRight(watches[i].displayName(false,true,false,14,6),24) << cOff << watches[i].statment << "   ";

            if (i < watches.size()-1) {
                watches.at(i).statment.nodeBal = getNodeBal(watches.at(i).balanceHistory, watches[i].address, block.blockNumber);
                cout << padLeft(wei_2_Ether(bni_2_Str(watches[i].statment.nodeBal)),23);
                if (!watches[i].statment.balanced()) {

                    cout << " " << bRed << padLeft(wei_2_Ether(bni_2_Str(watches[i].statment.difference())),23) << cOff << " " << redX;
                    if (report_bals) {
                        ostringstream os;
                        os << block.blockNumber << "\t" << watches[i].address << "\t" << watches[i].statment.endBal << "\n";
                        appendToAsciiFile("./balance_import.txt", os.str().c_str());
                    }
                    nOutOfBal++;
                } else {
                    cout << " " << greenCheck;
                }
            }
            cout << "\r\n";
        }
        cout << string_q(23, ' ') << string_q(125, '-') << "\r\n";
        cout << padRight("Total:",24) << total << " ";
        cout << greenCheck;
        cout << cOff;
        if (transStats.nAccountedFor)
            cout << "\r\n" << bYellow << string_q(180, '-') << "\r\n";
        cout.flush();
    }

    if (no_check)
        nOutOfBal = 0;

    if (debugger_on) {
        if (nOutOfBal ||
            single_on ||
#ifdef DEBUGGER_ON
            debugFile() ||
#endif
            esc_hit) {
            if (!enterDebugger(block)) {
                esc_hit = false;
                return false;  // quit
            }
        }
    } else {
        if (single_on || nOutOfBal) {
            if (!autocorrect_on)
                cout << "\r\nHit enter to continue, 'c' to correct and continue, or 'q' to quit >> ";
            else
                cout << "\r\nHit enter to continue or 'q' to quit >> ";
            int ch = getchar();
            if (ch == 'q') {
                esc_hit = false;
                return false;
            } else if (ch == 'c') {
                for (size_t i = 0 ; i < watches.size() ; i++)
                    watches.at(i).statment.correct();
            } else if (ch == 'n') {
                single_on = !single_on;
            }
        }
    }

    if (autocorrect_on) {
        for (size_t i = 0 ; i < watches.size() ; i++)
            watches.at(i).statment.correct();
    }
    esc_hit = false;
    return true;
}
