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
#include "etherlib.h"

extern bool handle_division(void);
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);
    handle_division();
    etherlib_cleanup();
}

#if 0
#include "summary.h"
uint64_t lineNum = 0;
map<address_t, uint64_t> theMap;
//----------------------------------------------------------------
bool visitRecord(const char* line, void* data) {
    CStringArray parts;
    explode(parts, line, '\t');
    theMap[parts[0]] += str_2_Uint(parts[1]);
    cerr << padNum9T(lineNum++) << "\t" << theMap.size() << "\t" << parts[0] << "\r";
    cerr.flush();
    return true;
}

//----------------------------------------------------------------
bool visitFile(const string_q& path, void* data) {
    if (endsWith(path, "/")) {
        return forEveryFileInFolder(path + "*", visitFile, data);
    }
    lineNum = 0;
    forEveryLineInAsciiFile(path, visitRecord, data);
    return true;
}

//----------------------------------------------------------------
string_q folder = "/Volumes/Tokenomics Data/addresses/collected/";
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);
    forEveryFileInFolder(folder, visitFile, nullptr);
    for (auto item : theMap) {
        cout << item.first << "\t" << item.second << endl;
    }
    etherlib_cleanup();
}
#endif

#if 0
// The following code does summaries for theCount.csv
//----------------------------------------------------------------
int main(int argc, const char* argv[]) {
    CSummary::registerClass();
    etherlib_init(quickQuitHandler);

    CSummaryArray summaries;
    CStringArray lines;
    asciiFileToLines("store/theCounts.csv", lines);
    CStringArray fields;
    for (auto line : lines) {
        if (fields.size() == 0) {
            explode(fields, line, ',');
            // for (auto field : fields)
            //     cout << field << endl;
        } else {
            CSummary summary;
            summary.parseCSV(fields, line);
            summaries.push_back(summary);
        }
    }

    CSummary prev, total;
    for (auto summary : summaries) {
        // cout << summary.count << "\t" << (summary.total - prev.total) << "\t"
        //      << ((summary.total - prev.total) - summary.count) << endl;
        prev = summary;
        total += summary;
        if (!(summary.end % 100000)) {
            cout << total.end << "," << total.count << "," << total.total << endl;
            total = CSummary();
        }
    }

    etherlib_cleanup();
    return 1;
}
#endif
