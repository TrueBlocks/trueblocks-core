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

//-----------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        options.isText = (expContext().exportFmt & (TXT1 | CSV1));
        if (options.isText && !options.timestamps && options.requests.size() == 0) {
            LOG_INFO("No results");

        } else {
            if (options.firstOut)
                cout << exportPreamble(expContext().fmtMap["header"], GETRUNTIME_CLASS(CBlock));

            if (options.requests.size() > 0) {
                options.applyFilter();

            } else if (options.timestamps) {
                getTimestampAt(1);  // freshens timestamp file
                forEveryTimestamp(visitBlock, &options);
            }

            cout << exportPostamble(options.errors, expContext().fmtMap["meta"]);
        }
    }

    etherlib_cleanup();
    return 0;
}

//-----------------------------------------------------------------------
bool visitBlock(CBlock& block, void* data) {
    COptions* opt = reinterpret_cast<COptions*>(data);
    if (opt->check || opt->fix) {
        if (opt->checker.expected == 0) {
            opt->checker.expected++;
            return true;
        }
        return checkTimestamp(block, data);
    }

    if (opt->isText) {
        cout << block.Format(expContext().fmtMap["format"]) << endl;

    } else {
        if (!opt->firstOut)
            cout << "," << endl;
        cout << "  ";
        indent();
        block.toJson(cout);
        unindent();
    }
    opt->firstOut = false;
    if (isTestMode() && (++opt->cnt > 100))
        return false;

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

            // TODO(tjayrush): this should be in the library so every request for zero block gets a valid
            // blockNumber
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
            if (!visitBlock(block, this)) {
                return;
            }

        } else if (request.first == "date") {
            if (lookupDate(block, (timestamp_t)str_2_Uint(request.second), getBlockProgress(BP_CLIENT).client)) {
                if (!visitBlock(block, this)) {
                    return;
                }
            } else {
                LOG_WARN("Could not find a block at date " + request.second);
            }
        }
    }
}

//-----------------------------------------------------------------------
bool checkTimestamp(CBlock& block, void* data) {
    COptions* opt = (COptions*)data;
    CTimeStamper* c = &opt->checker;
    LOG_INFO("Checking block (", block.blockNumber, ", ", block.timestamp, ")\r");

    bool err = false;
    if (block.blockNumber != c->expected)
        err = true;

    if (block.blockNumber != c->prevBn + 1)
        err = true;

    else if (block.timestamp <= c->prevTs)
        err = true;

    if (err) {
        ostringstream os;
        CBlock blk;
        getBlock_light(blk, c->expected);
        os << "chifra when --timestamps --fix " << c->expected << " ";
        os << "# should be (" << blk.blockNumber << ", " << blk.timestamp << ") ";
        os << "is (" << block.blockNumber << ", " << block.timestamp << ") ";
        LOG_WARN(os.str());
    }

    c->expected++;
    c->prevBn = block.blockNumber;
    c->prevTs = block.timestamp;

    return true;
}
