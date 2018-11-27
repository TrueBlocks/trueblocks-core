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

extern bool visitTransaction(CTransaction& trans, void *data);
//--------------------------------------------------------------
int main(int argc, const char *argv[]) {

    etherlib_init();

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    CStringArray commands;
    explode(commands, options.commandList, '\n');
    for (auto command : commands) {
        if (!options.parseArguments(command))
            return 0;
        forEveryTransactionInList(visitTransaction, &options, options.transList.queries);
    }

    size_t cnt = 0;
    cout << "[";
    for (auto l : options.logs) {
        if (cnt++ > 0)
            cout << ",";
        l.doExport(cout);
    }
    if (cnt && options.rawLogs.size())
        cout << ",\n";

    cnt = 0;
    for (auto str : options.rawLogs) {
        if (cnt++ > 0)
            cout << ",\n";
        cout << str;
    }
    cout << "]";

    return 0;
}

//--------------------------------------------------------------
bool visitTransaction(CTransaction& trans, void *data) {
    COptions *opt = (COptions*)data;
    if (contains(trans.hash, "invalid")) {
        ostringstream os;
        os << cRed << "{ \"error\": \"The logs for transaction ";
        os << nextTokenClear(trans.hash, ' ') << " were not found.\" }" << cOff;
        opt->rawLogs.push_back(os.str());
        return true;
    }

    if (opt->isRaw) {
        // Note: this call is redundant. The transaction is already populated (if it's valid), but we need the raw data)
        string_q raw;
        //queryRawLogs(<#string_q &results#>, <#const address_t &addr#>, <#uint64_t fromBlock#>, <#uint64_t toBlock#>)(raw, trans.getValueByName("hash"));
        raw = substitute(raw,"[{\"jsonrpc\":\"2.0\",\"result\":[", "");
        raw = substitute(raw,"],\"id\":1}\n]", "");
        opt->rawLogs.push_back(raw);
        return true;
    }

    for (auto l : trans.receipt.logs)
        opt->logs.push_back(l);

    return true;
}
