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

//----------------------------------------------------------------
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);
    CIndexHashMap hashes;
    loadHashes(hashes, "finalized");
    forEveryFileInFolder(getIndexPath("finalized"), visitFile, &hashes);
    etherlib_cleanup();
    return 1;
}

//----------------------------------------------------------------
bool visitFile(const string_q& path, void* data) {
    if (endsWith(path, '/')) {
        forEveryFileInFolder(path + "*", visitFile, data);
    } else {
        if (endsWith(path, ".bin")) {
            blknum_t end;
            timestamp_t unused2;
            blknum_t start = bnFromPath(path, end, unused2);
            CIndexHashMap *hashes = (CIndexHashMap*)data;
            CIndexArchive index(READING_ARCHIVE);
            if (index.ReadIndexFromBinary(path)) {
                cout << start << ",";
                cout << end << ",";
//                cout << index.header.magic << ",";
//                for (int i=0;i<32;i++)
//                    cout << index.header.hash[i];
//                cout << ",";
                cout << index.header.nAddrs << ",";
                cout << index.header.nRows << ",";
                cout << fileSize(path) << ",";
                cout << fileSize(substitute(substitute(path, "finalized", "blooms"),".bin",".bloom")) << ",";
                CIndexChunk chunk = hashes->operator[](start);
                cout << chunk.hash << "\n";

            }
        }
    }
    return !shouldQuit();
}
