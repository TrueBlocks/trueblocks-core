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

static char delim = ',';
//--------------------------------------------------------------
bool bloomVisitFunc(const string_q& path, void* data) {
    if (endsWith(path, "/")) {
        return forEveryFileInFolder(path + "*", bloomVisitFunc, data);

    } else {
        if (!endsWith(path, ".bloom"))
            return true;

        blknum_t endBlock = NOPOS;
        blknum_t startBlock = bnFromPath(path, endBlock);
        blknum_t last = *(blknum_t*)data;
        if (last >= startBlock)
            return true;

        CBloomArray blooms;
        readBloomFromBinary(path, blooms);

        CIndexHeader header;
        string_q chunkPath = substitute(substitute(path, "blooms", "finalized"), ".bloom", ".bin");
        readIndexHeader(chunkPath, header);

        blknum_t nBlocks = (endBlock - startBlock) + 1;
        size_t nBlooms = blooms.size();
        size_t recordSize = (sizeof(uint32_t) + getBloomWidthInBytes());
        size_t totalAddrs = 0;
        size_t totalBits = 0;
        for (auto bloom : blooms) {
            totalBits += bloom.nBitsHit();
            totalAddrs += bloom.nInserted;
        }
        string_q checkSize = sizeof(uint32_t) + (nBlooms * recordSize) == fileSize(path) ? greenCheck : redX;

        ostringstream os;
        os << startBlock << delim;
        os << endBlock << delim;
        os << nBlocks << delim;
        os << header.nAddrs << delim;
        os << header.nRows << delim;
        os << totalBits << delim;
        os << double_2_Str(nBlocks ? float(header.nAddrs) / float(nBlocks) : 0., 3) << delim;
        os << double_2_Str(nBlocks ? float(header.nRows) / float(nBlocks) : 0., 3) << delim;
        os << double_2_Str(header.nAddrs ? float(header.nRows) / float(header.nAddrs) : 0., 3) << delim;
        os << double_2_Str(totalBits ? float(totalAddrs) / float(totalBits) : 0., 3) << delim;
        os << nBlooms << delim;
        os << recordSize << delim;
        os << fileSize(path) << delim;
        os << fileSize(chunkPath) << delim;
        os << double_2_Str(fileSize(path) ? float(fileSize(chunkPath)) / float(fileSize(path)) : 0., 3) << delim;
        os << date_2_Ts(fileLastModifyDate(path)) << delim;
        os << date_2_Ts(fileLastModifyDate(chunkPath));
        cout << os.str();
        cout << checkSize << delim;
        cout << ((header.nAddrs == totalAddrs) ? greenCheck : redX);
        cout << endl;
        appendToAsciiFile(getCachePath("tmp/chunk_stats.csv"), os.str() + "\n");
    }

    return true;
}

//----------------------------------------------------------------
bool COptions::handle_stats() {
    cout << "start" << delim;
    cout << "end" << delim;
    cout << "nBlks" << delim;
    cout << "nAddrs" << delim;
    cout << "nApps" << delim;
    cout << "nBits" << delim;
    cout << "nA/nB" << delim;
    cout << "nP/nB" << delim;
    cout << "nP/nA" << delim;
    cout << "nA/Bt" << delim;
    cout << "nBlooms" << delim;
    cout << "recWid" << delim;
    cout << "bloomSz" << delim;
    cout << "chunkSz" << delim;
    cout << "comp" << delim;
    cout << "bloomTs" << delim;
    cout << "chunkTs";
    cerr << "checkSize" << delim;
    cerr << "checkCount";
    cout << endl;

    CStringArray lines;
    asciiFileToLines(getCachePath("tmp/chunk_stats.csv"), lines);
    for (auto line : lines) {
        cout << line << endl;
    }
    blknum_t last = 0;
    if (lines.size() > 0)
        last = str_2_Uint(lines[lines.size() - 1]);
    return forEveryFileInFolder(indexFolder_blooms, bloomVisitFunc, &last);
}
