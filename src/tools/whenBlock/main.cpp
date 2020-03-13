/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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

extern bool forEveryTimestamp(BLOCKVISITFUNC func, void* data, uint64_t start, uint64_t count, uint64_t skip);
//-----------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    options.first = true;
    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        options.isText = (expContext().exportFmt & (TXT1 | CSV1));
        if (options.isText && !options.timestamps && options.requests.size() == 0) {
            LOG_INFO("No results");

        } else {
            if (options.first)
                cout << exportPreamble(expContext().fmtMap["header"], GETRUNTIME_CLASS(CBlock));
            if (options.requests.size() > 0) {
                options.applyFilter();  // for (auto item : options.items)
                                        // visitBlock(item.second, &options);

            } else if (options.timestamps) {
                forEveryTimestamp(visitBlock, &options, 0, options.stop,
                                  2 * (options.skip == NOPOS ? 1 : options.skip));
            }
        }
        cout << exportPostamble(options.errors, expContext().fmtMap["meta"]);
    }

    etherlib_cleanup();
    return 0;
}

//-----------------------------------------------------------------------
bool visitBlock(CBlock& block, void* data) {
    COptions* opt = reinterpret_cast<COptions*>(data);
    if (opt->isText) {
        cout << block.Format(expContext().fmtMap["format"]) << endl;

    } else {
        if (!opt->first)
            cout << "," << endl;
        cout << "  ";
        incIndent();
        block.doExport(cout);
        decIndent();
    }
    opt->first = false;
    return true;
}

//---------------------------------------------------------------
bool lookupDate(const COptions* options, CBlock& block, const timestamp_t& ts) {
    time_q date = ts_2_Date(ts);

    // speed up
    blknum_t start = 1, stop = getLatestBlock_client();
    if (date.GetYear() >= 2020) {
        start = 9193265;
    } else if (date.GetYear() >= 2019) {
        start = 6988614;
        stop = 9193265;
    } else if (date.GetYear() >= 2018) {
        start = 4832685;
        stop = 6988614;
    } else if (date.GetYear() >= 2017) {
        start = 2912406;
        stop = 4832685;
    } else if (date.GetYear() >= 2016) {
        start = 778482;
        stop = 2912406;
    }

    block.timestamp = ts;
    bool ret = findTimestamp_binarySearch(block, start, stop, true);
    if (!isTestMode()) {
        cerr << "\r";
        cerr.flush();
    }
    return ret;
}

//-------------------------------------------------------------------------
bool forEveryTimestamp(BLOCKVISITFUNC func, void* data, uint64_t start, uint64_t count, uint64_t skip) {
    if (!func)
        return false;

    uint32_t* tsArray = NULL;
    size_t nItems;
    if (!loadTimestampFile(&tsArray, nItems))
        return false;
    if (!tsArray)  // it may not have failed, but there may be no timestamps
        return false;

    for (size_t bn = start; bn < count; bn += skip) {
        CBlock block;
        block.blockNumber = tsArray[bn];
        block.timestamp = tsArray[bn + 1];
        bool ret = (*func)(block, data);
        if (!ret) {
            // Cleanup and return if user tells us to
            ASSERT(tsArray)
            delete[] tsArray;
            return false;
        }
    }

    ASSERT(tsArray)
    delete[] tsArray;
    return true;
}

//--------------------------------------------------------------------------------
void COptions::applyFilter() {
    //    items.reserve(requests.size() + 10)
    for (auto request : requests) {
        CBlock block;
        if (request.first == "block") {
            string_q bnStr = nextTokenClear(request.second, '|');
            if (request.second == "latest") {
                if (isTestMode())
                    continue;  // latest changes per test, so skip
            }

            getBlock_light(block, str_2_Uint(bnStr));

            // TODO(tjayrush): this should be in the library so every request for zero block gets a valid blockNumber
            if (block.blockNumber == 0) {
                blknum_t bn = str_2_Uint(bnStr);
                if (bn != 0) {
                    // We've been asked to find a block that is in the future...estimate 14 blocks
                    block.timestamp = istanbulTs + timestamp_t(14 * (bn - instanbulBlock));
                    block.blockNumber = bn;
                    request.second += " (est)";

                } else {
                    block.timestamp = blockZeroTs;
                }
            }
            block.name = request.second;
            visitBlock(block, this);

        } else if (request.first == "date") {
            if (lookupDate(this, block, (timestamp_t)str_2_Uint(request.second)))
                visitBlock(block, this);
            else
                LOG_WARN("Could not find a block at date " + request.second);
        }
    }
}
