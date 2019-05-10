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
        LOG4("raw");
        // Note: this call is redundant. The transaction is already populated (if it's valid), but we need the raw data)
        string_q raw;
        queryRawLogs(raw, trans.blockNumber, trans.blockNumber, trans.to);
        CRPCResult generic;
        generic.parseJson3(raw);
        CLogEntry log;
        log.parseJson3(generic.result);
        UNHIDE_FIELD(CLogEntry, "blockHash");
        UNHIDE_FIELD(CLogEntry, "blockNumber");
        UNHIDE_FIELD(CLogEntry, "removed");
        UNHIDE_FIELD(CLogEntry, "transactionHash");
        UNHIDE_FIELD(CLogEntry, "transactionIndex");
        UNHIDE_FIELD(CLogEntry, "transactionLogIndex");
        UNHIDE_FIELD(CLogEntry, "type");
        LOG4(log.Format());
        opt->rawLogs.push_back(log.Format());
        EXIT_NOMSG(true);
    }

    for (auto l : trans.receipt.logs) {
        LOG4("Pushing: ", l);
        opt->logs.push_back(l);
    }

    EXIT_NOMSG(true);
}
