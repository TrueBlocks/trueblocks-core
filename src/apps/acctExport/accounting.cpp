/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
bool COptions::openIncomeStatement(const CBlock& block) {
    if (!accounting_on)
        return true;

    for (size_t i = 0 ; i < watches.size() ; i++) {
        CAccountWatch *w = &watches.at(i);
        w->qbis.inflow = w->qbis.outflow = w->qbis.gasCostInWei = 0;
        if (i < watches.size()-1) {
            w->qbis.nodeBal = getNodeBal(w->balanceHistory, w->address, block.blockNumber-1);
            bigint_t diff = (w->qbis.nodeBal - w->qbis.endBal);
            if (diff != 0) {
                cout << "\n" << string_q(5, '-') << " WARNING " << string_q(166, '-') << "\n";
                cout
                    << w->address << " is out of balance by "
                    << wei_2_Ether(bni_2_Str(diff))
                    << " ether at the start of block "
                    << cYellow << block.blockNumber << "\n";
                cout << string_q(180, '-') << "\n";
            }
        }
    }
    tx_nAccountedFor = 0;
    return true;
}

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
            watches.at(fWhich).qbis.gasCostInWei += (trans->receipt.gasUsed * trans->gasPrice);
            tx_nAccountedFor++;
        }
        return true;
    }

    watches.at(fWhich).qbis.outflow += trans->value;
    if (fWhich != watches.size() - 1)
        watches.at(fWhich).qbis.gasCostInWei += (trans->receipt.gasUsed * trans->gasPrice);
    watches.at(tWhich).qbis.inflow += trans->value;
    tx_nAccountedFor++;
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

    watches.at(fWhich).qbis.outflow += trace->action.value;
    // gas is paid by originating account
    //    if (fWhich != watches.size() - 1)
    //        watches[fWhich].qbis.gasCostInWei += (trans->receipt.gasUsed * trace->gasPrice);
    watches.at(tWhich).qbis.inflow += trace->action.value;
    tx_nAccountedFor++;
    //    tBuffer.addItem(trace->blockNumber, trace->transactionIndex, trace->hash);
    return true;
}

//-----------------------------------------------------------------------
bool COptions::closeIncomeStatement(const CBlock& block) {

    // ...If the user is not accounting or nothing worth reporting happened...
    if (!accounting_on || !tx_nAccountedFor) {
        return true;
    }

    CIncomeStatement total;
    for (size_t i = 0 ; i < watches.size() ; i++)
        total += watches[i].qbis;

    bool outOfBal = false;
//TODO(tjayrush) This should be an option
    if (true) { //total.inflow > 0 || (total.outflow + total.gasCostInWei) > 0) {
        CIncomeStatement header;
        header.begBal = header.endBal = -1;

        cout << string_q(23, ' ') << " Profit and Loss for block #" << block.blockNumber << " (" << ts_2_Date(block.timestamp).Format(FMT_JSON) << ")\n";
        cout << padCenter("",23) << " " << header << "  " << padCenter("nodeBal",34) << "\n";
        for (size_t i = 0 ; i < watches.size() ; i++) {
            watches.at(i).qbis.blockNum = block.blockNumber;
            watches.at(i).qbis.begBal = watches[i].qbis.endBal;
            watches.at(i).qbis.endBal = (watches[i].qbis.begBal + watches[i].qbis.inflow - watches[i].qbis.outflow - watches[i].qbis.gasCostInWei);

            cout << watches[i].color << padRight(watches[i].displayName(false,true,false,14,6),24) << watches[i].qbis << "   ";

            if (i < watches.size()-1) {
                watches.at(i).qbis.nodeBal = getNodeBal(watches.at(i).balanceHistory, watches[i].address, block.blockNumber);
                cout << padLeft(wei_2_Ether(bni_2_Str(watches[i].qbis.nodeBal)),23);
                if (!watches[i].qbis.balanced()) {

                    cout << " " << padLeft(wei_2_Ether(bni_2_Str(watches[i].qbis.difference())),23) << " " << redX;
                    if (report_bals) {
                        ostringstream os;
                        os << block.blockNumber << "\t" << watches[i].address << "\t" << watches[i].qbis.endBal << "\n";
                        appendToAsciiFile("./balance_import.txt", os.str().c_str());
                    }
                    outOfBal = true;
                } else {
                    cout << " " << greenCheck;
                }
            }
            cout << "\n";
        }
        cout << string_q(23, ' ') << string_q(125, '-') << "\n";
        cout << padRight("Total:",24) << total << " ";
        cout << greenCheck;
        if (tx_nAccountedFor)
            cout << "\n" << string_q(180, '-') << "\n";
        cout.flush();
    }

    if (outOfBal) {
        if (!autocorrect_on)
            cout << "\nHit enter to continue, 'c' to correct and continue, or 'q' to quit >> ";
        else
            cout << "\nHit enter to continue or 'q' to quit >> ";
        int ch = getchar();
        if (ch == 'q') {
            return false;
        } else if (ch == 'c') {
            for (size_t i = 0 ; i < watches.size() ; i++)
                watches.at(i).qbis.correct();
        }
    }

    if (autocorrect_on) {
        for (size_t i = 0 ; i < watches.size() ; i++)
            watches.at(i).qbis.correct();
    }
    return true;
}
