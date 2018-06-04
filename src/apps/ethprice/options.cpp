/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//---------------------------------------------------------------------------------------------------
CParams params[] = {
    CParams("-at:<timestamp>",                 "Report the price since nearest five minutes to the given timestamp"),
    CParams("-current",                        "Report on the current price (i.e. -at:now)"),
    CParams("-freshen",                        "Freshen database (append new data)"),
    CParams("-period:<5|15|30|*120|240|1440>", "Display prices in this increment. One of [5|15|30|120*|240|1440]"),
    CParams("-pair:<val>",                     "Which price pair to freshen or list (see Poloniex)"),
    CParams("",                                "Freshen and/or display Ethereum price data and other purposes.\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    if (!standardOptions(command))
        return false;

    Init();
    while (!command.empty()) {

        SFString arg  = nextTokenClear(command, ' ');
        // do not collapse
        if (arg == "-c" || arg == "--current")
            arg = "-a:now";
        SFString orig = arg;

        if (arg == "-f" || arg == "--freshen") {
            freshen = true;

        } else if (arg.startsWith("-a:") || arg.startsWith("--at:")) {
            arg = orig.Substitute("-a:","").Substitute("--at:","");
            if (arg == "now") {
                at = toTimestamp(Now());
            } else {
                at = toTimestamp(arg);
                if (!isUnsigned(arg))
                    return usage("Timestamp expected: " + orig);
            }

        } else if (arg.startsWith("-p:") || arg.startsWith("--period:")) {
            arg = orig.Substitute("-p:","").Substitute("--period:","");
            freq = newUnsigned32(arg);
            if (!isUnsigned(arg) || freq % 5)
                return usage("Positive multiple of 5 expected: " + orig);

        } else if (arg.startsWith("--pair:")) {
            arg = orig.Substitute("--pair:","");
            source.pair = arg;

        } else {
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }
        }
    }

    if (!fileExists(source.getDatabasePath())) {
        if (verbose)
            cerr << "Establishing price database cache.\n";
        establishFolder(source.getDatabasePath());
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;

    freshen = false;
    freq = 120;
    hour = 0;
    at = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    needsOption = true;
    Init();
}

//--------------------------------------------------------------------------------
SFString COptions::postProcess(const SFString& which, const SFString& str) const {

    if (which == "options") {
        //return str.Substitute("address_list block_list", "<address> [address...] [block...]")
        //        .Substitute("-l|", "-l fn|");

    } else if (which == "notes" && (verbose || COptions::isReadme)) {

        //SFString ret;
        //ret += "[{addresses}] must start with '0x' and be forty characters long.\n";
        //ret += "[{block_list}] may be a space-separated list of values, a start-end range, a [{special}], or any combination.\n";
        //ret += "This tool retrieves information from the local node or the ${FALLBACK} node, if configured (see documentation).\n";
        //ret += "If the queried node does not store historical state, the results are undefined.\n";
        //ret += "[{special}] blocks are detailed under " + cTeal + "[{whenBlock --list}]" + cOff + ".\n";
        //return ret;
    }
    return str;
}
