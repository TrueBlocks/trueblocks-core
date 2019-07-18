/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_balances(void) {

    ENTER4("handle_" + mode);
    nodeRequired();
    //if (!nodeHasBalances())
    //    EXIT_USAGE("This function requires an Ethereum archive node to work.");

    if (addrs.empty())
        EXIT_USAGE("This function requires an address.");

    if (!freshen_internal(FM_PRODUCTION, addrs, "", freshen_flags))
        EXIT_FAIL("'chifra export' freshen_internal returned false");

    for (auto addr : addrs) {
        ostringstream os;
        if (getGlobalConfig("chifra")->getConfigBool("api", "remote_bals", false)) {
            string_q cmd = "/Users/jrush/src.GitHub/quickBlocks/build/get_balances.sh " + addr;
            cout << cmd << endl;
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
            CStringArray inLines, out;
            asciiFileToLines(fn, inLines);
            out.reserve(inLines.size());
            for (auto line : inLines) {
                CUintArray parts;
                explode(parts, line, '\t');
                cout << addr << "\t" << parts[0] << "\t" << parts[1] << "\t" << wei_2_Ether(bnu_2_Str(getBalanceAt(addr, parts[0]))) << endl;
            }
            ::remove(fn.c_str());
        }
    }

    EXIT_NOMSG4(true);
}
