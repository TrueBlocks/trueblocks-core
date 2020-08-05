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
#include "acctlib.h"

uint64_t cnt = 0;
#define indexFolder_sorted (getIndexPath("sorted/"))
//----------------------------------------------------------------
bool visitFile(const string_q& path, void* data) {
    if (endsWith(path, '/')) {
        return forEveryFileInFolder(path + "*", visitFile, data);

    } else {
        string_q asciiFn = substitute(path, indexFolder_sorted, "./ascii/");
        bool asciiExists = fileExists(asciiFn);

        string_q binaryFn = substitute(substitute(path, indexFolder_sorted, "./f inalized/"), ".txt", ".bin");
        bool binaryExists = fileExists(binaryFn);

        string_q bloomFn = substitute(substitute(path, indexFolder_sorted, "./b looms/"), ".txt", ".bloom");
        bool bloomExists = fileExists(bloomFn);

        if (asciiExists && binaryExists && bloomExists) {
            cerr << cGreen << cnt++ << ": Completed: " << cTeal << path << cOff << endl;
            return true;
        }

        CStringArray lines;
        cerr << endl << cGreen << cnt++ << ": Processing file: " << cTeal << path << cOff << endl;

        cerr << "\tReading..." << endl;
        asciiFileToLines(path, lines);

        cerr << "\tSorting..." << endl;
        sort(lines.begin(), lines.end());

        lockSection(true);
        if (asciiExists)
            cerr << "\t" << greenCheck << " Ascii file " << asciiFn << " exists..." << endl;
        if (!asciiExists) {
            writeIndexAsAscii(asciiFn, lines);
            asciiExists = true;
            string_q zipPath = substitute(asciiFn, "/ascii/", "/ascii.zips/");
            if (!fileExists(zipPath)) {
                cerr << "\tCopying zip files..." << endl;
                string_q cmd = "cp -p " + asciiFn + " " + zipPath + " ; ";
                cerr << "\t" << cmd << endl;
                if (!doCommand(cmd).empty())
                    cerr << "\t" << redX << " command failed." << endl;
                cmd = " cd ./ascii.zips/ ; gzip *.txt ;";
                if (!doCommand(cmd).empty())
                    cerr << "\t" << redX << " command failed." << endl;
            }
        }

        if (binaryExists)
            cerr << "\t" << greenCheck << " Binary file " << binaryFn << " exists..." << endl;
        if (bloomExists)
            cerr << "\t" << greenCheck << " Bloom file " << bloomFn << " exists..." << endl;
        if (!binaryExists || !bloomExists) {
            // writes both bin and bloom
            writeIndexAsBinary(binaryFn, lines);

            // We now copy again to build the IPFS cache file. We zip the data first to make it as small as possible
            cerr << "\tCopying zip files..." << endl;
            string_q zipPath = substitute(binaryFn, "/finalized/", "/finalized.zips/");
            string_q cmd = "cp -p " + binaryFn + " " + zipPath + " ; ";
            cerr << "\t" << cmd << endl;
            if (!doCommand(cmd).empty())
                cerr << "\t" << redX << " command failed.";
            cmd = " cd ./finalized.zips/ ; gzip *.bin ;";
            if (!doCommand(cmd).empty())
                cerr << "\t" << redX << " command failed." << endl;

            zipPath = substitute(bloomFn, "/blooms/", "/blooms.zips/");
            cmd = "cp -p " + bloomFn + " " + zipPath + " ; ";
            cerr << "\t" << cmd << endl;
            if (!doCommand(cmd).empty())
                cerr << "\t" << redX << " command failed.";
            cmd = " cd ./blooms.zips/ ; gzip *.bloom ;";
            if (!doCommand(cmd).empty())
                cerr << "\t" << redX << " command failed." << endl;
        }
        lockSection(false);
    }

    return !shouldQuit();
}

//----------------------------------------------------------------
int main(int argc, const char* argv[]) {
    nodeNotRequired();
    etherlib_init(defaultQuitHandler);

    forEveryFileInFolder(indexFolder_sorted, visitFile, NULL);
    cout << "Done..." << endl;

    etherlib_cleanup();
}

