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
#include "acctlib.h"

#define indexFolder_sorted_v2 (getCachePath("addr_index/sorted/"))
//----------------------------------------------------------------
bool visitFile(const string_q& path, void *data) {

    if (endsWith(path, '/')) {
        return forEveryFileInFolder(path + "*", visitFile, data);

    } else {

        string_q asciiFn = substitute(path, indexFolder_sorted_v2, "./output-ascii/");
        bool asciiExists = fileExists(asciiFn);

        string_q binaryFn = substitute(substitute(path, indexFolder_sorted_v2, indexFolder_finalized_v2), ".txt", ".bin");
        bool binaryExists = fileExists(binaryFn);

        string_q bloomFn = substitute(substitute(path, indexFolder_sorted_v2, indexFolder_blooms_v2), ".txt", ".bloom");
        bool bloomExists = fileExists(bloomFn);

        CStringArray lines;
        if (!asciiExists || !binaryExists || !bloomExists) {
            cerr << endl << cGreen << "Processing file: " << cTeal << path << cOff << endl;

            cerr << "\tReading..." << endl;
            asciiFileToLines(path, lines);

            cerr << "\tSorting..." << endl;
            sort(lines.begin(), lines.end());
        }

        if (asciiExists) {
            cerr << "\t" << greenCheck << " Ascii file " << asciiFn << " exists..." << endl;
        } else {
            writeIndexAsAscii(asciiFn, lines);
            // We now copy again to build the IPFS cache file. We zip the data first to make it as small as possible
            cerr << "\tCopying zip files..." << endl;
            string_q zipPath = substitute(asciiFn, "/output-ascii/", "/output-azips/");
            string_q cmd = "cp -p " + asciiFn + " " + zipPath + " ; ";
            cerr << "\t" << cmd << endl;
            if (!doCommand(cmd).empty())
                cerr << "\t" << redX << " command failed.";
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
            string_q zipPath = substitute(binaryFn, indexFolder_finalized_v2, "./output-bzips/");
            string_q cmd = "cp -p " + binaryFn + " " + zipPath + " ; ";
            cerr << "\t" << cmd << endl;
            if (!doCommand(cmd).empty())
                cerr << "\t" << redX << " command failed.";

            zipPath = substitute(bloomFn, indexFolder_blooms_v2, "./output-czips/");
            cmd = "cp -p " + bloomFn + " " + zipPath + " ; ";
            cerr << "\t" << cmd << endl;
            if (!doCommand(cmd).empty())
                cerr << "\t" << redX << " command failed.";
        }
    }

    return true;
}

//----------------------------------------------------------------
int main(int argc, const char *argv[]) {
    nodeNotRequired();
    etherlib_init(defaultQuitHandler);

    forEveryFileInFolder(indexFolder_sorted_v2, visitFile, NULL);
    cout << "Done..." << endl;

    etherlib_cleanup();
}

#if 0
void stats(const CNewBloomArray& blooms, const string_q& path) {
    cout << "d    nTotAddrs:     " << N_RAND_ADDR << endl;
    cout << "d    maxInserts:    " << MAX_INSERTS << endl;
    cout << "d    nBytes/bloom:  " << bloom_nt::BYTE_SIZE << " " << sizeof(bloom_nt) << endl;
    cout << "d    nBits/bloom:   " << bloom_nt::BIT_SIZE() << endl;
    cout << "d    Size:          " << blooms.size() << endl;
    for (auto bloom : blooms) {
        cout << "d        nInserted: " << bloom.nInserted << endl;
        cout << "d        onBits:    " << bloom.nBitsHit();
        cout << " (" << (double(bloom.nBitsHit())/double(bloom_nt::BIT_SIZE()))*100. << "%)" << endl;
    }
    cout << "d    File:          " << fileSize(path) << " bytes)" << endl;
}
#endif
