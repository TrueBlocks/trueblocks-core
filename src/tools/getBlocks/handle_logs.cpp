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
    if (!processFastPath())
        return blocks.forEveryBlockNumber(visitBlockLogs, this);
    return true;
}

//---------------------------------------------------------------------------
bool visitBlockLogs(uint64_t num, void* data) {
    LOG_INFO("Processing block ", num, "\r");

    COptions* opt = (COptions*)data;  // NOLINT
    opt->logFilter.fromBlock = num;
    opt->logFilter.toBlock = num;
    CBlock block;
    getBlock_light(block, num);
    string_q result;
    queryRawLogs(result, opt->logFilter);
    CRPCResult generic;
    generic.parseJson3(result);
    result = generic.result;
    CLogEntry log;
    while (log.parseJson3(result)) {
        if (opt->articulate) {
            opt->abi_spec.loadAbiFromEtherscan(log.address);
            opt->abi_spec.articulateLog(&log);
        }
        manageFields("CFunction:message", !log.articulatedLog.message.empty());
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
        log = CLogEntry();
    }

    return !shouldQuit();
}

//---------------------------------------------------------------------------
bool COptions::processFastPath(void) {
    if (blocks.start == 0 && blocks.stop == 0)
        return false;  // not a range
    if (!logFilter.isFastPath())
        return false;  // no emitter and no topics
    return false;
}
