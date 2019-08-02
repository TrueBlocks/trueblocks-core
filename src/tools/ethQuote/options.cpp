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
#include "options.h"

//---------------------------------------------------------------------------------------------------
// BEG_CODE_OPTIONS
static const COption params[] = {
//#define NEW_CODE
#ifdef NEW_CODE
    COption2("at", "", "<timestamp>", 0, "Report the price since nearest five minutes to the given timestamp"),
    COption2("current", "", "", OPT_FLAG, "Report on the current price (i.e. -at:now)"),
    COption2("data", "", "", OPT_FLAG, "Export prices as JSON data"),
    COption2("freshen", "", "", OPT_FLAG, "Freshen database (append new data)"),
    COption2("period", "", "<5|15|30|*120|240|1440>", 0, "Display prices in this increment. One of [5|15|30|120*|240|1440]"),
    COption2("pair", "", "<val>", 0, "Which price pair to freshen or list (see Poloniex)"),
    COption2("", "", "", 0, "Freshen and/or display Ethereum price data and other purposes."),
#else // NEW_CODE
    COption("-at:<timestamp>",                 "Report the price since nearest five minutes to the given timestamp"),
    COption("-current",                        "Report on the current price (i.e. -at:now)"),
    COption("-data",                           "Export prices as JSON data"),
    COption("-freshen",                        "Freshen database (append new data)"),
    COption("-period:<5|15|30|*120|240|1440>", "Display prices in this increment. One of [5|15|30|120*|240|1440]"),
    COption("-pair:<val>",                     "Which price pair to freshen or list (see Poloniex)"),
    COption("",                                "Freshen and/or display Ethereum price data and other purposes.\n"),
#endif
};
static const size_t nParams = sizeof(params) / sizeof(COption);
// END_CODE_OPTIONS

extern const char* STR_DISPLAY;
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    bool noHeader = false;
    string_q format;
    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
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

    // Data wrangling
    // None

    // Display formatting
    switch (exportFmt) {
        case NONE1:
        case TXT1:
        case CSV1:
            format = getGlobalConfig()->getConfigStr("display", "format", format.empty() ? STR_DISPLAY : format);
            manageFields("CAccountName:" + cleanFmt(format, exportFmt));
            break;
        case API1:
        case JSON1:
            format = "";
            break;
    }
    expContext().fmtMap["format"] = expContext().fmtMap["header"] = cleanFmt(format, exportFmt);
    if (noHeader)
        expContext().fmtMap["header"] = "";

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
    registerOptions(nParams, params);

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
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
string_q COptions::postProcess(const string_q& which, const string_q& str) const {
    if (which == "options") {

    } else if (which == "notes" && (verbose || COptions::isReadme)) {
        string_q ret;
        ret += "Valid pairs include any pair from the public Poloniex's API here:|"
                "https://poloniex.com/public?command=returnCurrencies.\n";
        ret += "[{Note}]: Due to restrictions from Poloniex, this tool retrieves only 30 days of data|"
                "at a time. You must repeatedly run this command until the data is up-to-date.\n";
        return ret;
    }
    return str;
}

//-----------------------------------------------------------------------
const char *STR_DISPLAY =
"[{BLOCKNUMBER}]\t"
"[{TIMESTAMP}]\t"
"[{PRICE}]";
