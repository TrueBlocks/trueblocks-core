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
    for (auto r : options.receipts) {
        if (cnt++ > 0)
            cout << ",";
        r.doExport(cout);
    }
    if (cnt && options.rawReceipts.size())
        cout << ",\n";

    cnt = 0;
    for (auto str : options.rawReceipts) {
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
        os << cRed << "{ \"error\": \"The receipt for transaction ";
        os << nextTokenClear(trans.hash, ' ') << " was not found.\" }" << cOff;
        opt->rawReceipts.push_back(os.str());
        return true;
    }

    if (opt->isRaw) {
        string_q raw;
        queryRawReceipt(raw, trans.getValueByName("hash"));
        raw = substitute(raw,"{\"jsonrpc\":\"2.0\",\"result\":", "");
        raw = substitute(raw,",\"id\":1}", "");
        opt->rawReceipts.push_back(raw);
        return true;
    }

    opt->receipts.push_back(trans.receipt);
    return true;
}
