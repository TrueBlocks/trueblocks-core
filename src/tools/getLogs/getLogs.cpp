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
    etherlib_init(quickQuitHandler);

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;
        forEveryTransactionInList(visitTransaction, &options, options.transList.queries);
    }

    size_t cnt = 0;
    cout << "[";
    for (auto item : options.items) {
        if (cnt++ > 0)
            cout << ",";
        item.doExport(cout);
    }
    if (cnt && options.rawItems.size())
        cout << ",\n";

    cnt = 0;
    for (auto str : options.rawItems) {
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
        os << cRed << "{ \"error\": \"The item you requested (";
        os << nextTokenClear(trans.hash, ' ') << ") was not found.\" }" << cOff;
        opt->rawItems.push_back(os.str());
        return true;
    }

    if (!opt->isRaw) {
        for (auto log : trans.receipt.logs)
            opt->items.push_back(log);
        return true;
    }

    string_q fields =
        "CBlock:blockHash,blockNumber|"
        "CTransaction:to,from,blockHash,blockNumber|"
        "CReceipt:to,from,blockHash,blockNumber,transactionHash,transactionIndex,cumulativeGasUsed,logsBloom,root|"
        "CLogEntry:blockHash,blockNumber,transactionHash,transactionIndex,transactionLogIndex,removed,type";
    manageFields(fields, true);

    string_q result;
    queryRawLogs(result, trans.blockNumber, trans.blockNumber);
    if (opt->isVeryRaw) {
        opt->rawItems.push_back(result);
        return true;
    }
    CRPCResult generic;
    generic.parseJson3(result);
    CBlock bl;
    CTransaction tt; tt.pBlock = &bl;
    CReceipt receipt; receipt.pTrans = &tt;
    CLogEntry log; log.pReceipt = &receipt;
    while (log.parseJson3(generic.result)) {
        if (log.getValueByName("transactionIndex") == uint_2_Str(trans.transactionIndex))
            opt->rawItems.push_back(log.Format());
        log = CLogEntry();
        log.pReceipt = &receipt;
    }

    return true;
}
