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

//--------------------------------------------------------------
bool bloomVisitFunc(const string_q& path, void* data) {
    if (endsWith(path, "/")) {
        return forEveryFileInFolder(path + "*", bloomVisitFunc, data);

    } else {
        CBloomArray blooms;
        readBloomFromBinary(path, blooms);

        char delim = '\t';
        blknum_t endBlock = NOPOS;
        blknum_t startBlock = bnFromPath(path, endBlock);
        blknum_t nBlocks = (endBlock - startBlock) + 1;
        size_t size = fileSize(path);
        size_t nRecords = blooms.size();

        static bool been_here = false;
        if (!been_here) {
            cout << "startBlock" << delim;
            cout << "endBlock" << delim;
            cout << "nBlocks" << delim;
            cout << "fileSize" << delim;
            cout << "nRecords" << delim;
            cout << "recordSize" << delim;
            cout << "bloomWidthInBytes" << delim;
            cout << "bloomWidthInBits" << delim;
            cout << "maxAddrsInBloom" << delim;
            cout << "nibbleWidth" << delim;
            cout << "K" << endl;
            been_here = true;
        }

        cout << startBlock << delim;
        cout << endBlock << delim;
        cout << nBlocks << delim;
        cout << size << delim;
        cout << nRecords << delim;
        cout << 0 << delim;
        cout << BLOOM_WIDTH_IN_BYTES << delim;
        cout << BLOOM_WIDTH_IN_BITS << delim;
        cout << MAX_ADDRS_IN_BLOOM << delim;
        cout << NIBBLE_WID << delim;
        cout << K << endl;

        //         uint32_t nBlooms;
        //         bloomCache.Read(nBlooms);
        //         for (size_t i = 0; i < nBlooms; i++) {
        //             bloom_t bloom;
        //             bloomCache.Read(bloom.nInserted);
        //             bloomCache.Read(bloom.bits, sizeof(uint8_t), BLOOM_WIDTH_IN_BYTES);
        //             blooms.push_back(bloom);
        //         }

        //         cout << blooms.size() << ",";
        //         cout << (float(fileSize(path) - sizeof(uint64_t)) / float(blooms.size())) << endl;
    }

    return true;
}

//----------------------------------------------------------------
bool COptions::handle_extract() {
    return forEveryFileInFolder(indexFolder_blooms, bloomVisitFunc, nullptr);
}

// //----------------------------------------------------------------
// typedef struct {
//   public:
//     char addr[42];
//     char t1;
//     char bn[9];
//     char t2;
//     char tx[5];
//     char nl;
// } Thing;

// //----------------------------------------------------------------
// int sortByBlock(const void* v1, const void* v2) {
//     Thing* t1 = (Thing*)v1;
//     Thing* t2 = (Thing*)v2;

//     int ret = strncmp(t1->bn, t2->bn, 9);
//     if (ret)
//         return ret;

//     ret = strncmp(t1->addr, t2->addr, 42);
//     if (ret)
//         return ret;

//     ret = strncmp(t1->tx, t2->tx, 5);
//     return ret;
// }

// //----------------------------------------------------------------
// bool visitFile(const string_q& path, void* data) {
//     if (endsWith(path, '/')) {
//         forEveryFileInFolder(path + "*", visitFile, data);

//     } else {
//         blknum_t end;
//         blknum_t start = bnFromPath(path, end);

//         CArchive file(READING_ARCHIVE);
//         if (file.Lock(path, modeReadOnly, LOCK_NOWAIT)) {
//             uint64_t size = fileSize(path);
//             uint64_t nRecords = size / sizeof(Thing);
//             Thing* t = new Thing[nRecords + 1];
//             if (t) {
//                 bzero(t, sizeof(Thing) * (nRecords + 1));
//                 file.Read(t, sizeof(Thing), nRecords);
//                 for (size_t i = 0; i < nRecords; i++) {
//                     t[i].t1 = t[i].t2 = t[i].nl = '\0';
//                 }
//                 qsort(t, nRecords, sizeof(Thing), sortByBlock);
//                 blknum_t last = str_2_Uint(t->bn);
//                 ostringstream os;
//                 for (size_t i = 0; i < nRecords; i++) {
//                     blknum_t bn = str_2_Uint(t[i].bn);
//                     if (bn != last) {
//                         string_q f = padNum9(last);
//                         f = "./" + f.substr(0, 3) + "/" + f.substr(3, 3) + "/" + f + ".txt";
//                         establishFolder(f);
//                         cerr << f << endl;
//                         stringToAsciiFile(f, os.str());
//                         os.str("");
//                         os.clear();
//                         last = bn;
//                     }
//                     os << t[i].addr << "\t" << t[i].bn << "\t" << t[i].tx << endl;
//                 }
//                 string_q f = t[nRecords - 1].bn;
//                 f = "./" + f.substr(0, 3) + "/" + f.substr(3, 3) + "/" + f + ".txt";
//                 establishFolder(f);
//                 stringToAsciiFile(f, os.str());
//                 delete[] t;
//             }
//         }
//     }
//     return true;
// }
