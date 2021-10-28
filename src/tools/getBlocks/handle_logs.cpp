/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
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

extern bool visitBlockLogs(uint64_t num, void* data);
extern const char* STR_FMT_BLOCKLOGS;
//---------------------------------------------------------------------------
bool COptions::handle_logs(void) {
    return blocks.forEveryBlockNumber(visitBlockLogs, this);
}

//------------------------------------------------------------
bool visitBlockLogs(uint64_t num, void* data) {
    LOG_INFO("Processing block ", num, "\r");

    COptions* opt = (COptions*)data;  // NOLINT
    CBlock block;
    getBlock_light(block, num);
    // return block.forEveryTransaction(visitTransactionLogs, data);
    string_q url =
        "curl -s --data "
        "'{"
        "\"method\":\"erigon_getLogsByHash\","
        "\"params\":[\"" +
        block.hash +
        "\"],"
        "\"id\":1,"
        "\"jsonrpc\":\"2.0\""
        "}' "
        "-H \"Content-Type: application/json\" -X POST localhost:23456 | jq";
    string_q result = doCommand(url);
    CRPCResult generic;
    generic.parseJson3(result);
    result = generic.result;
    CLogEntry log;
    while (log.parseJson3(result)) {
        if (opt->logFilter.passes(log)) {
            log.blockNumber = block.blockNumber;
            log.blockHash = block.hash;
            log.timestamp = block.timestamp;
            bool isText = (expContext().exportFmt & (TXT1 | CSV1));
            if (!opt->firstOut) {
                if (!isText)
                    cout << ",";
                cout << endl;
            }
            cout << log.Format(expContext().fmtMap["format"]);
            opt->firstOut = false;
        }
        log = CLogEntry();
    }
    return !shouldQuit();
}
