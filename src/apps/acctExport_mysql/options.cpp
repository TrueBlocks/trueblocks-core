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
static COption params[] = {
    COption("-mysql",           "use mysql as the ultimate destination of the data"),
    COption("-ma(x)Rows:<num>", "the maximum number of rows of data to write"),
    COption("-m(i)grate",       "drop and then re-create the database"),
    COption("-configs",         "process configs to update the data in the monitor configs"),
    COption("-prices",          "process prices and write to the database"),
    COption("-json",            "export json file"),
    COption("",                 "Index transactions for a given Ethereum address (or series of addresses).\n"),
};
static size_t nParams = sizeof(params) / sizeof(COption);

extern string_q cleanFmt(const string_q& str);
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    Init();
    bool doMigrate = false;
    bool doConfig = false;
    bool doPrices = false;
    while (!command.empty()) {
        string_q arg = nextTokenClear(command, ' ');
        if (arg == "-m" || arg == "--mysql") {
            sql_on = true;

        } else if (startsWith(arg, "-x:") || startsWith(arg, "--maxRows:")) {
            nextTokenClear(arg,':');
            if (!isNumeral(arg))
                return usage("Please provide a number for --maxRows option. Quitting...");
            maxRows = str_2_Uint(arg);

        } else if (startsWith(arg, "-j") || startsWith(arg, "--json")) {
            json = true;

        } else if (startsWith(arg, "-p") || startsWith(arg, "--prices")) {
            doPrices = true;

        } else if (arg == "-i" || arg == "--migrate") {
            doMigrate = true;

        } else if (arg == "-c" || arg == "--configs") {
            doConfig = true;

        } else if (startsWith(arg, '-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }
        }
    }

    if (doMigrate || doConfig || doPrices) {

        cerr << cYellow << "Are you sure you want to drop the database and re-build it? This is a non-recoverable operation. (y=yes)\n> " << cOff;
        int ch = getchar();
        if (ch != 'y' && ch != 'Y') {
            cerr << "You quit the operation. The database is intact. Quitting.\n";
            cerr.flush();
            exit(0);
        }

        if (doMigrate)
            processMigrate();

        if (doConfig)
            processConfigs(*this);

        if (doPrices)
            processPrices();

        cerr.flush();
        exit(0);
    }

    if (!fileExists("./config.toml"))
        return usage("The config.toml file was not found. Are you in the right folder? Quitting...\n");

    CToml toml("./config.toml");
    if (!loadWatches(toml))
        return false;
//TODO(tjayrush): Do I need this?
//    if (!loadBalances())
//        return usage("Could not get balances. Quitting...");

    transFmt    = cleanFmt(toml.getConfigStr ("formats", "trans_fmt",    ""));
    transExport = cleanFmt(toml.getConfigStr ("formats", "trans_export", transFmt));
    sql_on      =          toml.getConfigBool("data",    "use_sql",     false) || sql_on;

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;

    transFmt = "";
    transExport = "";
    json = false;
    maxRows = NOPOS;
    sql_on = false;
    minArgs = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
    barLen(80);
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//-----------------------------------------------------------------------
bool COptions::loadWatches(const CToml& toml) {

    // Note: this call does no checks on the data
    loadWatchList(toml, watches, "list");

    if (watches.size() == 0)
        return usage("Empty list of watches. Quitting...\n");

    // Check the watches for validity
    for (uint32_t i = 0 ; i < watches.size() ; i++) {

        const CAccountWatch *watch = &watches[i];
        if (!isAddress(watch->address))
            return usage("Invalid watch address " + watch->address + "\n");

        if (watch->name.empty())
            return usage("Empty watch name " + watch->name + "\n");
    }
    return true;
}

//-----------------------------------------------------------------------
//TODO(tjayrush): Do I need this?
//bool COptions::loadBalances(void) {
//
//    for (uint32_t i = 0 ; i < watches.size() ; i++) {
//        CAccountWatch *watch = &watches[i];
//        watch->nodeBal = getNodeBal(watch->balanceHistory, watch->address, watch->firstBlock-1);
//    }
//    return true;
//}

//-----------------------------------------------------------------------
string_q cleanFmt(const string_q& str) {
    return (substitute(substitute(substitute(substitute(substitute((str), "\\n\\\n", "\\n"), "\n", ""), "\\n", "\n"), "\\t", "\t"), "\\r", "\r"));
}
