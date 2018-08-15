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

extern const char* STR_NAME_DATA;
//-----------------------------------------------------------------------
int main(int argc, const char *argv[]) {

    getCurlContext()->provider = "None";  // This will run without a node
    etherlib_init();

    COptions options;
    options.setProgramName(basename((char*)argv[0]));

    // If --help or -h is the first item in the arg list, the user is asking for help on this command,
    // otherwise, we send every command option to the underlying command
    CStringArray h = { "--help", "-h" };
    for (auto i : h) {
        if (argc == 2 && i == argv[1])
            return usage();
    }

    if (!options.prepareArguments(argc, argv))
        return 0;

    while (!options.commandList.empty()) {
        string_q command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;
        string_q cmd = options.cmd + " " + options.args + " --redir_stderr";
        if (isTestMode())
            cout << "calling: " << cmd << "\n";
        cout << unconvert(doCommand(cmd)) << "\n";
    }
    return 0;
}

//--------------------------------------------------------------------------------
string_q convert(const string_q& str) {
    string_q ret = str;
    replace(ret, "blocks",   "getBlock");
    replace(ret, "txs",      "getTrans");
    replace(ret, "receipts", "getReceipt");
    replace(ret, "logs",     "getLogs");
    replace(ret, "traces",   "getTraces");
    replace(ret, "names",    "ethName");
    replace(ret, "when",     "whenBlock");
    replace(ret, "where",    "whereBlock");
    return ret;
}

//--------------------------------------------------------------------------------
string_q unconvert(const string_q& str) {
    string_q ret = str;
    replace(ret, "getBlock",   "qblocks blocks");
    replace(ret, "getTrans",   "qblocks txs");
    replace(ret, "getReceipt", "qblocks receipts");
    replace(ret, "getLogs",    "qblocks logs");
    replace(ret, "getTraces",  "qblocks traces");
    replace(ret, "ethName",    "qblocks names");
    replace(ret, "whenBlock",  "qblocks when");
    replace(ret, "whereBlock", "qblocks where");
    return ret;
}
