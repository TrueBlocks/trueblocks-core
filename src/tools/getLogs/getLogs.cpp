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

    ENTER("visitTransaction");
    LOG4(trans.blockNumber, ".", trans.transactionIndex);

    COptions *opt = (COptions*)data;
    if (contains(trans.hash, "invalid")) {
        ostringstream os;
        os << cRed << "{ \"error\": \"The logs for transaction ";
        os << nextTokenClear(trans.hash, ' ') << " were not found.\" }" << cOff;
        opt->rawLogs.push_back(os.str());
        LOG4("invalid");
        EXIT_NOMSG(true);
    }

    if (opt->isRaw) {
        string_q fields =
            "CBlock:blockHash,blockNumber|"
            "CTransaction:to,from,blockHash,blockNumber|"
            "CReceipt:to,from,blockHash,blockNumber,transactionHash,transactionIndex|"
            "CLogEntry:blockHash,blockNumber,transactionHash,transactionIndex,transactionLogIndex,removed,type";
        manageFields(fields, true);

        string_q raw;
        queryRawLogs(raw, trans.blockNumber, trans.blockNumber);
        if (opt->isVeryRaw) {
            opt->rawLogs.push_back(raw);
            EXIT_NOMSG(true);
        }
        CRPCResult generic;
        generic.parseJson3(raw);
        CBlock bl;
        CTransaction tt; tt.pBlock = &bl;
        CReceipt rr; rr.pTrans = &tt;
        CLogEntry log; log.pReceipt = &rr;
        while (log.parseJson3(generic.result)) {
            if (log.getValueByName("transactionIndex") == uint_2_Str(trans.transactionIndex))
                opt->rawLogs.push_back(log.Format());
            log = CLogEntry();
            log.pReceipt = &rr;
        }
        EXIT_NOMSG(true);
    }

    for (auto l : trans.receipt.logs) {
        LOG4("Pushing: ", l);
        opt->logs.push_back(l);
    }

    EXIT_NOMSG(true);
}
