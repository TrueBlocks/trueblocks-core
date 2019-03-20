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

extern bool lookupDate(const COptions *options, CBlock& block, const timestamp_t& ts);
//---------------------------------------------------------------
int main(int argc, const char *argv[]) {
    etherlib_init(quickQuitHandler);

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        for (auto value : options.requests) {
            string_q mode = nextTokenClear(value, ':');
            string_q special;
            if (mode == "special") {
                mode = "block";
                special = nextTokenClear(value, '|');
                if (str_2_Uint(value) > getLastBlock_client()) {
                    cerr << "The block number you requested (";
                    cerr << cTeal << special << ": " << value << cOff;
                    cerr << ") is after the latest block (";
                    cerr << cTeal << (isTestMode() ? "TESTING" : uint_2_Str(getLastBlock_client())) << cOff;
                    cerr << "). Quitting...\n";
                    return 0;
                }
            }

            CBlock block;
            if (mode == "block") {
                queryBlock(block, value, false);

            } else if (mode == "date") {
                bool found = lookupDate(&options, block, (timestamp_t)str_2_Uint(value));
                if (!found)
                    return 0;
            }

            // special case for the zero block
            if (block.blockNumber == 0)
                block.timestamp = 1438269960;

            string_q def = (options.asData ?
                                    "[{BLOCKNUMBER}]\\t[{TIMESTAMP}]\\t[{DATE}]\\n" :
                                    "block #[{BLOCKNUMBER}][ : {TIMESTAMP}][ : {DATE}]\\n");
            string_q fmt = getGlobalConfig("whenBlock")->getDisplayStr(options.asData, def);
            // we never want to print JSON
            if (fmt.empty()) fmt = substitute(def, "\\n" , "\n");
            if (verbose && !special.empty()) {
                string_q sp = "(" + special + ")";
                replace(fmt, "{BLOCKNUMBER}", "{BLOCKNUMBER} " + sp);
            }
            cout << block.Format(fmt);
        }
    }
    return 0;
}

//--------------------------------------------------------------
bool findTimestamp_binarySearch(CBlock& block, size_t first, size_t last) {

    string_q t("|/-\\|/-\\");
    static int i = 0;
    if (!isTestMode()) { cerr << "\r" << cGreen << t[(i++%8)] << " working" << cOff; cerr.flush(); }

    if (last > first) {
        size_t mid = first + ((last - first) / 2);
        CBlock b1, b2;
        getBlock(b1, mid);
        getBlock(b2, mid+1);
        bool atMid  = (b1.timestamp <= block.timestamp);
        bool atMid1 = (b2.timestamp <= block.timestamp);
        if (atMid && !atMid1) {
            block = b1;
            return true;
        } else if (!atMid) {
            // we're too high, so search below
            return findTimestamp_binarySearch(block, first, mid-1);
        }
        // we're too low, so search above
        return findTimestamp_binarySearch(block, mid+1, last);
    }
    getBlock(block, first);
    return true;
}

//---------------------------------------------------------------
bool lookupDate(const COptions *options, CBlock& block, const timestamp_t& ts) {
    time_q date = ts_2_Date(ts);

    // speed up
    blknum_t start = 1, stop = getLastBlock_client();
    if (date.GetYear() >= 2019) {
        start = 6988614;
    } else if (date.GetYear() >= 2018) {
        start = 4832685; stop = 6988614;
    } else if (date.GetYear() >= 2017) {
        start = 2912406; stop = 4832685;
    } else if (date.GetYear() >= 2016) {
        start = 778482; stop = 2912406;
    }

    block.timestamp = ts;
    bool ret = findTimestamp_binarySearch(block, start, stop);
    if (!isTestMode()) { cerr << "\r"; cerr.flush(); }
    return ret;
}
