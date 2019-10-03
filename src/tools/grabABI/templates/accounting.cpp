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

    for (auto& watch : watches) {
        watch.statement.inflow = watch.statement.outflow = watch.statement.gasCostInWei = 0;
        if (isAddress(watch.address)) {
            watch.statement.curBalance = get NodeBal(watch.stateHistory, watch.address, block.blockNumber-1);
            bigint_t diff = (watch.statement.curBalance - watch.statement.endBal);
            if (!no_check && (diff != 0)) {
                single_on = true;
                string_q c1 = watch.color, c2 = cOff;
                cout << "\r\n" << bRed << string_q(5, '-') << " WARNING " << string_q(166, '-') << "\r\n";
                cout
                << c1 << watch.address << c2 << " is out of balance by "
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
    size_t cnt = 0;
    for (auto watch : watches) {
        if (containsI(trans->to, watch.address))
            tWhich = cnt;
        if (containsI(trans->from, watch.address))
            fWhich = cnt;
        cnt++;
    }
    if (tWhich == watches.size() - 1) {
        cnt = 0;
        for (auto watch : watches) {
            if (containsI(trans->receipt.contractAddress, watch.address))
                tWhich = cnt;
            cnt++;
        }
    }

    // Nothing to record if there was an error, but we do have to account for gas
    if (trans->isError) { // || trans->value == 0)
        if (fWhich != watches.size() - 1) {
            watches.at(fWhich).statement.gasCostInWei += (trans->receipt.gasUsed * trans->gasPrice);
            transStats.nAccountedFor++;
        }
#ifdef DEBUGGER_ON
        tBuffer.addItem(trans->blockNumber, trans->transactionIndex, trans->hash);
#endif
        return true;
    }

    watches.at(fWhich).statement.outflow += trans->value;
    if (fWhich != watches.size() - 1)
        watches.at(fWhich).statement.gasCostInWei += (trans->receipt.gasUsed * trans->gasPrice);
    watches.at(tWhich).statement.inflow += trans->value;
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

    watches.at(fWhich).statement.outflow += trace->action.value;
    // gas is paid by originating account
    //    if (fWhich != watches.size() - 1)
    //        watches[fWhich].statement.gasCostInWei += (trans->receipt.gasUsed * trace->gasPrice);
    watches.at(tWhich).statement.inflow += trace->action.value;
    transStats.nAccountedFor++;
    //    tBuffer.addItem(trace->blockNumber, trace->transactionIndex, trace->hash);
    return true;
}

//-------------------------------------------------------------------------
ostream& thing(ostream& os, const CIncomeStatement& item, uint64_t theWidth) {
    if (sizeof(item) != 0) {  // do this to always go through here, but avoid a warning
        string_q spc = "  ";
        if (item.begBal == item.endBal && item.begBal == -1) {
            os <<  padCenter("begBal",  theWidth) << spc
            << padCenter("inFlow",  theWidth) << spc
            << padCenter("outFlow", theWidth) << spc
            << padCenter("gasCost", theWidth) << spc
            << padCenter("endBal",  theWidth);
        } else {
            os << (item.begBal       > 0 ? cGreen  : bBlack) << padLeft( wei_2_Ether(bni_2_Str( item.begBal       )), theWidth) << bBlack << spc;  // NOLINT
            os << (item.inflow       > 0 ? cYellow : ""    ) << padLeft( wei_2_Ether(bni_2_Str( item.inflow       )), theWidth) << bBlack << spc;  // NOLINT
            os << (item.outflow      > 0 ? cYellow : ""    ) << padLeft( wei_2_Ether(bni_2_Str( item.outflow      )), theWidth) << bBlack << spc;  // NOLINT
            os << (item.gasCostInWei > 0 ? cYellow : ""    ) << padLeft( wei_2_Ether(bni_2_Str( item.gasCostInWei )), theWidth) << cOff   << spc;  // NOLINT
            os << (item.endBal       > 0 ? cGreen  : bBlack) << padLeft( wei_2_Ether(bni_2_Str( item.endBal       )), theWidth);  // NOLINT
        }
        { return os; }
    }

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
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
        total += watches[i].statement;
    }

    size_t nOutOfBal = 0;
    //TODO(tjayrush) This should be an option
    if (true) { //total.inflow > 0 || (total.outflow + total.gasCostInWei) > 0) {
        CIncomeStatement header;
        header.begBal = header.endBal = -1;

        cout << cOff << string_q(theWidth+1, ' ') << cWhite << " Profit and Loss for block #" << block.blockNumber << " (" << ts_2_Date(block.timestamp).Format(FMT_JSON) << ")\r\n";
        //        cout << bBlack << string_q(theWidth+1, ' ') << BG << string_q(155, '-') << cOff << "\r\n";
        cout << padCenter("",theWidth+1) << " " << BG;
        thing(cout, header, theWidth+1);
        cout << padCenter("curBalance",theWidth+(theWidth/2)) << cOff << "\r\n";
        //        cout << bBlack << string_q(theWidth+1, ' ') << string_q(155, '-') << cOff << "\r\n";
        for (size_t i = 0 ; i < watches.size() ; i++) {
            watches.at(i).statement.blockNum = block.blockNumber;
            watches.at(i).statement.begBal = watches[i].statement.endBal;
            watches.at(i).statement.endBal = (watches[i].statement.begBal + watches[i].statement.inflow - watches[i].statement.outflow - watches[i].statement.gasCostInWei);

            cout << watches[i].color << padRight(watches[i].displayName(false,true,false,14,6),theWidth+2) << cOff;
            thing(cout, watches[i].statement, theWidth);
            cout << "   ";
            //cout << watches[i].statement << "   ";

            if (i < watches.size()-1) {
                watches.at(i).statement.curBalance = get NodeBal(watches.at(i).stateHistory, watches[i].address, block.blockNumber);
                cout << padLeft(wei_2_Ether(bni_2_Str(watches[i].statement.curBalance)),theWidth+1);
                if (!watches[i].statement.balanced()) {

                    cout << " " << bRed << padLeft(wei_2_Ether(bni_2_Str(watches[i].statement.difference())),theWidth+1) << cOff << " " << redX;
                    if (report_bals) {
                        ostringstream os;
                        os << block.blockNumber << "\t" << watches[i].address << "\t" << watches[i].statement.endBal << "\n";
                        appendToAsciiFile("./balance_import.txt", os.str().c_str());
                    }
                    nOutOfBal++;
                } else {
                    cout << " " << greenCheck;
                }
            }
            cout << "\r\n";
        }
        cout << string_q(theWidth+1, ' ') << string_q(125, '-') << "\r\n";
        cout << padRight("Total:",theWidth+2) << total << " ";
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
                    watches.at(i).statement.correct();
            } else if (ch == 'n') {
                single_on = !single_on;
            }
        }
    }

    if (autocorrect_on) {
        for (size_t i = 0 ; i < watches.size() ; i++)
            watches.at(i).statement.correct();
    }
    esc_hit = false;
    return true;
}
