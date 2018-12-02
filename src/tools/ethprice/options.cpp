/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
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
static COption params[] = {
    COption("-at:<timestamp>",                 "Report the price since nearest five minutes to the given timestamp"),
    COption("-current",                        "Report on the current price (i.e. -at:now)"),
    COption("-data",                           "Export prices as JSON data"),
    COption("-freshen",                        "Freshen database (append new data)"),
    COption("-period:<5|15|30|*120|240|1440>", "Display prices in this increment. One of [5|15|30|120*|240|1440]"),
    COption("-pair:<val>",                     "Which price pair to freshen or list (see Poloniex)"),
    COption("",                                "Freshen and/or display Ethereum price data and other purposes.\n"),
};
static size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        // do not collapse
        if (arg == "-c" || arg == "--current")
            arg = "-a:now";
        string_q orig = arg;

        if (arg == "-f" || arg == "--freshen") {
            freshen = true;

        } else if (arg == "-d" || arg == "--data") {
            // we don't have to do anything, simply handling the option
            // enables the behavour. Don't remove.

        } else if (startsWith(arg, "-a:") || startsWith(arg, "--at:")) {
            arg = substitute(substitute(orig, "-a:", ""), "--at:", "");
            if (arg == "now") {
                at = date_2_Ts(Now());
            } else {
                at = str_2_Ts(arg);
                if (!isUnsigned(arg))
                    return usage("Timestamp expected: " + orig);
            }

        } else if (startsWith(arg, "-p:") || startsWith(arg, "--period:")) {
            arg = substitute(substitute(orig, "-p:", ""), "--period:", "");
            freq = str_2_Uint(arg);
            if (!isUnsigned(arg) || freq % 5)
                return usage("Positive multiple of 5 expected: " + orig);

        } else if (startsWith(arg, "--pair:")) {
            arg = substitute(orig, "--pair:", "");
            source.pair = arg;

        } else {
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }
        }
    }

    string_q unused;
    if (!fileExists(source.getDatabasePath(unused))) {
        if (verbose)
            cerr << "Establishing price database cache.\n";
        establishFolder(source.getDatabasePath(unused));
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    arguments.clear();
    paramsPtr = params;
    nParamsRef = nParams;

    freshen = false;
    freq = 120;
    at = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    needsOption = true;
    Init();
}

//--------------------------------------------------------------------------------
string_q COptions::postProcess(const string_q& which, const string_q& str) const {

    if (which == "options") {
        // return substitute(
        //       substitute(str, "address_list block_list", "<address> [address...] [block...]"), "-l|", "-l fn|");

    } else if (which == "notes" && (verbose || COptions::isReadme)) {

        // string_q ret;
        // ret += "[{addresses}] must start with '0x' and be forty characters long.\n";
        // ret += "[{block_list}] may be a space-separated list of values, a start-end range, a [{special}], "
        //          "or any combination.\n";
        // ret += "This tool retrieves information from the local node or the ${FALLBACK} node, if configured "
        //          "(see documentation).\n";
        // ret += "If the queried node does not store historical state, the results are undefined.\n";
        // ret += "[{special}] blocks are detailed under " + cTeal + "[{whenBlock --list}]" + cOff + ".\n";
        // return ret;
    }
    return str;
}
