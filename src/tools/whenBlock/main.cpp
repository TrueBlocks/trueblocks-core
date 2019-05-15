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
#include "options.h"

//-----------------------------------------------------------------------
int main(int argc, const char *argv[]) {
    etherlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    bool first = true;
    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        bool isText = (options.exportFmt & (TXT1|CSV1));
        if (isText && options.items.size() == 0) {
            LOG_INFO("No results");

        } else {
            for (auto item : options.items) {
                if (first)
                    cout << exportPreamble(options.exportFmt, expContext().fmtMap["header"], item.second.getRuntimeClass());
                if (isText)
                    cout << item.second.Format(expContext().fmtMap["format"]) << endl;
                else {
                    if (!first)
                        cout << "," << endl;
                    cout << "  ";
                    incIndent();
                    item.second.doExport(cout);
                    decIndent();
                }
                first = false;
            }
        }
        cout << exportPostamble(options.exportFmt, expContext().fmtMap["meta"]);
    }

    etherlib_cleanup();
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
