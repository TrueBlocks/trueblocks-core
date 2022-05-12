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
                if (!options.no_update)
                    getTimestampAt(options.latest.blockNumber);  // freshens timestamp file but otherwise ignored
                forEveryTimestamp(visitBlock, &options);
                if (options.corrections.size() > 0) {
                    options.applyCorrections();
                }
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

    opt->cnt++;
    if (isTestMode()) {
        if (opt->cnt > 100)
            return false;
        if (contains(block.name, "(est)"))
            return true;
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

    return true;
}

extern void estBnFromTs(CBlock& block, timestamp_t ts);
extern string_q estTsFromBn(CBlock& block, blknum_t bn);
//--------------------------------------------------------------------------------
void COptions::applyFilter() {
    for (auto request : requests) {
        CBlock block;
        if (request.first == "block") {
            string_q bnStr = nextTokenClear(request.second, '|');
            if (request.second == "latest") {
                if (isTestMode())
                    continue;  // latest changes per test, so skip
            }

            getBlockHeader(block, str_2_Uint(bnStr));

            // TODO(tjayrush): this should be in the library so every request for zero block
            // TODO(tjayrush): gets a valid blockNumber
            if (block.blockNumber == 0) {
                blknum_t bn = str_2_Uint(bnStr);
                if (bn != 0) {
                    // We've been asked to find a block that is in the future...estimate 14 second blocks
                    block = latest;
                    request.second += estTsFromBn(block, bn);

                } else {
                    block.timestamp = blockZeroTs();
                }
            }
            block.name = request.second;
            if (!visitBlock(block, this)) {
                return;
            }

        } else if (request.first == "date") {
            timestamp_t ts = (timestamp_t)str_2_Uint(request.second);
            if (ts > latest.timestamp) {
                cerr << "Timestamp after latest block " << request.second << endl;
                block = latest;
                estBnFromTs(block, ts);
                request.second += " (est)";
            } else {
                lookupDate(block, (timestamp_t)str_2_Uint(request.second), latest.blockNumber);
            }
            if (!visitBlock(block, this))
                return;
        }
    }
}

//#define TEST string_q("flat")
#define TEST string_q("onesec")
//#define TEST string_q("twosecs")
double estSeconds(blknum_t bn) {
    blknum_t fake = bn - 10700000;
    uint64_t period = fake / 100000;
    uint64_t inc = 2;
    if (TEST == "onesec") {
        inc = 1;
    }
    if (period < 37 || TEST == "flat")
        return 13.3;
    switch (period) {
        case 37:
            return 14 + (inc * 0);
        case 38:
            return 14 + (inc * 1);
        case 39:
            return 14 + (inc * 2);
        case 40:
            return 14 + (inc * 3);
        case 41:
            return 14 + (inc * 4);
    }
    return 14 + (inc * 5);
}

string_q estTsFromBn(CBlock& block, blknum_t bn) {
    if (block.blockNumber > bn)
        return "";
    // incoming block has 'latest' timestamp and blockNumber
    float needed = bn - block.blockNumber;
    block.timestamp = block.timestamp + timestamp_t(estSeconds(bn) * needed);
    block.blockNumber = bn;
    return " (est " + TEST + ")";
}

void estBnFromTs(CBlock& block, timestamp_t ts) {
    if (block.timestamp > ts)
        return;
    // incoming block has 'latest' timestamp and blockNumber
    timestamp_t needed = ts - block.timestamp;
    block.blockNumber = block.blockNumber + blknum_t(floor((needed / estSeconds(block.blockNumber)) + .5));
    block.timestamp = ts;
}
