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
    // First we process the fast path queries if there are any. A fast path is
    // a block range query with either a topic or an emitter or both in the
    // logFilter field. The fast path clears the fast path but leaves any
    // non-fast-path queries in place, so we always handle both
    processFastPath();

    // We always non-fast-path queries if there are any
    return blocks.forEveryBlockNumber(visitBlockLogs, this);
}

//---------------------------------------------------------------------------
size_t queryLogs(const CLogFilter& filter, void* data) {
    COptions* opt = (COptions*)data;  // NOLINT
    bool isText = (expContext().exportFmt & (TXT1 | CSV1));

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
bool visitBlockLogs(uint64_t num, void* data) {
    LOG_INFO("Processing log query ", num, "\r");

    COptions* opt = (COptions*)data;  // NOLINT
    opt->logFilter.fromBlock = num;
    opt->logFilter.toBlock = num;
    return queryLogs(opt->logFilter, data) > 0;
}

//---------------------------------------------------------------------------
bool COptions::processFastPath(void) {
    if (blocks.start == 0 && blocks.stop == 0)
        return false;  // not a range
    if (!logFilter.isFastPath())
        return false;  // no emitter and no topics

    // We can't ask the node for the entire range if it's really large (say > big_range), so
    // we break it up into big_range block mini-ranges
    size_t nRead = 0;
    for (blknum_t bn = blocks.start; bn < blocks.stop; bn += big_range) {
        logFilter.fromBlock = bn;
        logFilter.toBlock = min(blocks.stop, bn + big_range) - 1;
        LOG_INFO("Processing log query ", logFilter.fromBlock, " to ", logFilter.toBlock, "\r");
        nRead += queryLogs(logFilter, this);
    }
    blocks.start = blocks.stop = 0;  // we've processed this, so remove it

    return nRead > 0;
}
