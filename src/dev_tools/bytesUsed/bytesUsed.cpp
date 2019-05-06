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
#include "reporter.h"

extern bool countBytes(const string_q& path, void *data);
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

        cout << "blknum\tnFiles\tnBytes\ttFiles\ttBytes\t";
        cout << "nBlooms\tnBytes\ttBlooms\ttBytes" << endl;

        CReporter reporter;
        forEveryFileInFolder(blockFolder + "00/", countBytes, &reporter);
        cout << "block stats: " << reporter.blocks.stats() << "\n";
//        cout << "bloom stats: " << reporter.blooms.stats() << "\n";
    }
    return 0;
}

//-----------------------------------------------------------------------
bool countBytes(const string_q& path, void *data) {

    CReporter *counter = reinterpret_cast<CReporter*>(data);
    if (endsWith(path, "/")) {

        forEveryFileInFolder(path + "*", countBytes, data);
        counter->blocks.accumulate();
//        counter->blooms.accumulate();

        if (countOf(path, '/') == 8) { // we're at the bottom, so let's report
            CFilename fn(path);
            string_q blockNum = substitute(fn.getFullPath(), "/", "") + "000";
            blockNum = extract(blockNum, blockNum.length()-9, 9).c_str();
            cout << trimLeading(blockNum,'0') << "\t"
                    << counter->blocks.cur_nFiles << "\t"
                    << counter->blocks.cur_nBytes << "\t"
                    << counter->blocks.tot_nFiles << "\t"
                    << counter->blocks.tot_Bytes <<
            //"\t"
            //        << counter->blooms.cur_nFiles << "\t"
            //        << counter->blooms.cur_nBytes << "\t"
            //        << counter->blooms.tot_nFiles << "\t"
            //        << counter->blooms.tot_Bytes <<
                        "\n";
            cout.flush();

        }
        counter->blocks.reset();
        //counter->blooms.reset();

    } else {

        //counter->blooms.cur_nBytes += fileSize(path);
        //counter->blooms.cur_nFiles += 1;
        string_q blockPath = substitute(path, "/blooms/", "/blocks/");
        if (fileExists(blockPath)) {
            counter->blocks.cur_nBytes += fileSize(blockPath);
            counter->blocks.cur_nFiles += 1;
        }

    }

    return true;
}
