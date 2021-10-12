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
        if (!endsWith(path, ".bloom"))
            return true;

        CBloomArray blooms;
        readBloomFromBinary(path, blooms);

        char delim = '\t';

        static bool been_here = false;
        if (!been_here) {
            cout << "startBlock" << delim;
            cout << "endBlock" << delim;
            cout << "nBlocks" << delim;
            cout << "fileSize" << delim;
            cout << "nBlooms" << delim;
            cout << "recordSize" << delim;
            cout << "checkSize" << delim;
            cout << "nAddrs" << delim;
            cout << "nBits" << delim;
            cout << "avtBits" << endl;
            been_here = true;
        }

        blknum_t endBlock = NOPOS;
        blknum_t startBlock = bnFromPath(path, endBlock);
        blknum_t nBlocks = (endBlock - startBlock) + 1;
        size_t nBlooms = blooms.size();
        size_t recordSize = (sizeof(uint32_t) + getBloomWidthInBytes());
        string_q checkSize = sizeof(uint32_t) + (nBlooms * recordSize) == fileSize(path) ? greenCheck : redX;
        size_t size = fileSize(path);
        cout << startBlock << delim;
        cout << endBlock << delim;
        cout << nBlocks << delim;
        cout << size << delim;
        cout << nBlooms << delim;
        cout << recordSize << delim;
        size_t totalAddrs = 0;
        size_t totalBits = 0;
        for (auto bloom : blooms) {
            totalBits += bloom.nBitsHit();
            totalAddrs += bloom.nInserted;
        }
        cout << totalAddrs << delim;
        cout << totalBits << delim;
        cout << (totalBits ? float(totalAddrs) / float(totalBits) : 0) << endl;
        cout << checkSize << endl;
        CIndexHeader header;
        string_q chunkPath = substitute(substitute(path, "blooms", "finalized"), ".bloom", ".bin");
        readIndexHeader(chunkPath, header);
        cout << delim << fileExists(chunkPath) << delim << chunkPath << endl;
#if 0
        static size_t x = 0;
        x++;
        if ((x % 50))
            return true;
        CStringArray colors = {
            bGreen, bBlue, bTeal, bMagenta, bYellow, bWhite, cGreen, cBlue, cTeal, cMagenta, cYellow, cWhite,
        };
#define NN (2048)
        size_t cnt = 0;
        for (auto bloom : blooms) {
            // if (startBlock < 13000000)
            //     continue;
            for (size_t i = 0; i < getBloomWidthInBits(); i++) {
                if (bloom.isBitLit(i))
                    cout << colors[cnt % 12] << '1' << cOff;
                else
                    cout << '.';
                if (!(i % NN))
                    cout << " " << startBlock << endl;
                cout.flush();
                // usleep(10000);
            }
            cout << cRed << string_q(150, '=') << cOff << endl << endl;
            cnt++;
        }
        if (false) {
            for (auto bloom : blooms) {
                cout << delim << cnt++ << delim << bloom.nInserted;
                cout << delim;
                for (size_t i = 0; i < getBloomWidthInBytes(); i += NN) {
                    bloom.showBloom(cout, (i * NN), NN);
                    cout << endl << delim << delim << delim;
                }
                cout << cGreen << string_q(150, '=') << cOff << endl << endl;
            }
        }
#endif
    }

    return true;
}

//----------------------------------------------------------------
bool COptions::handle_extract() {
    if (blooms)
        return forEveryFileInFolder(indexFolder_blooms, bloomVisitFunc, nullptr);
    LOG_INFO("Not yet implemented");
    return true;
}
