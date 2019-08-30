/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
bool COptions::exportBalances(void) {

    ENTER("exportBalances");

    if (exportFmt == JSON1 && !freshenOnly)
        cout << "[";

    for (auto monitor : monitors) {
        CAppearanceArray_base apps;
        loadOneAddress(apps, monitor.address);
        CBalanceRecordArray balances;
        if (!nodeHasBalances()) {
            if (getGlobalConfig("acctExport")->getConfigBool("balances", "remote", false)) {
                ostringstream os;
                os << "/Users/jrush/src.GitHub/trueblocks-core/build/get_balances.sh ";
                os << monitor.address;
                os << (" --start " + uint_2_Str(scanRange.first));
                if (system(os.str().c_str())) { }

                string_q contents = asciiFileToString("/Users/jrush/Desktop/files/" + monitor.address + ".bals.txt");
                CStringArray lines;
                explode(lines, contents, '\n');
                for (auto line : lines) {
                    CBalanceRecord rec(line);
                    balances.push_back(rec);
                }
            } else {
                EXIT_FAIL("Balances not available.");
            }

        } else {
            for (size_t i = 0 ; i < apps.size() && !shouldQuit() && apps[i].blk < ts_cnt ; i++) {
                const CAppearance_base *item = &apps[i];
                if (inRange((blknum_t)item->blk, scanRange.first, scanRange.second)) {
                    CBalanceRecord rec;
                    rec.address = monitor.address;
                    rec.blockNum = item->blk;
                    rec.tx_id = item->txid;
                    rec.priorBalance = (item->blk == 0 ? 0 : getBalanceAt(rec.address, item->blk-1));
                    rec.balance = getBalanceAt(rec.address, item->blk);
                    balances.push_back(rec);
                    cerr << "   balance for " << rec.address << " at block " << rec.blockNum << ": " << rec.balance << " (" << i << " of " << items.size() << "\r";
                    cerr.flush();
                }
            }
        }

        bool first = true;
        size_t nExported = 0;
        for (auto record : balances) {
            nExported++;
            if (!freshenOnly) {
                if (exportFmt == JSON1 && !first)
                    cout << ", ";
                cout << record;
                first = false;
            }
        }

        // write the data to the hard drive so we can use it next time
    }

//    if (!freshenOnly)
//        cerr << "exported " << nExported << " of " << items.size() << " records." << string_q(45,' ') << "\n";
//    cerr.flush();
    if (exportFmt == JSON1 && !freshenOnly)
        cout << "]";

//    for (auto watch : monitors)
//        watch.writeLastExport(items[items.size()-1].blk);

    EXIT_NOMSG(true);
}

#if 0

    for (auto addr : addrs) {
        ostringstream os;
        if (getGlobalConfig("acctExport")->getConfigBool("api", "r emote_bals", false)) {
            string_q cmd = "/Users/jrush/src.GitHub/trueblocks-core/build/get_balances.sh " + addr;
            if (system(cmd.c_str())) { }  // Don't remove. Silences compiler warnings
                                          //            cout << "/Users/jrush/Desktop/files/" + addr + ".bals.txt";

        } else
        {
            string_q fn = "/tmp/results";
            os << "cacheMan " << " -d " << addr << " >" + fn + " ; ";
            LOG1("Calling " + os.str());
            if (isTestMode())
                cout << substitute(os.str(), getCachePath(""), "$BLOCK_CACHE/") << endl;
            else
                if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings

            CStringArray inLines;
            asciiFileToLines(fn, inLines);
            for (auto line : inLines) {
                CUintArray parts;
                explode(parts, line, '\t');
                cout << addr << "\t";
                cout << parts[0] << "\t";
                cout << parts[1] << "\t";
                if (parts[0] > 0)
                    cout << wei_2_Ether(bnu_2_Str(getBalanceAt(addr, parts[0]-1))) << "\t";
                else
                    cout << wei_2_Ether("0") << "\t";
                cout << wei_2_Ether(bnu_2_Str(getBalanceAt(addr, parts[0])));
                cout << endl;
            }
            ::remove(fn.c_str());
        }
    }

    EXIT_NOMSG4(true);
}
#endif

