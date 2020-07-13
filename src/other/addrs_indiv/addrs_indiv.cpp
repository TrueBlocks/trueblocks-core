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

typedef struct {
    CIndexHashMap hashes;
    CIndexHashMap bloom;
} Thing;

//----------------------------------------------------------------
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);
    Thing thing;
    loadHashes(thing.hashes, "finalized");
    loadHashes(thing.blooms, "blooms");
    cout << "start,end,nAddrs,nRows,fileSize,bloomSize,hash,bloom_hash\n";
    forEveryFileInFolder(getIndexPath("finalized"), visitFile, &thing);
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
            Thing *thing = (Thing*)data;
            CIndexArchive index(READING_ARCHIVE);
            if (index.ReadIndexFromBinary(path)) {
                cout << start << ",";
                cout << end << ",";
                cout << index.header.nAddrs << ",";
                cout << index.header.nRows << ",";
                cout << fileSize(path) << ",";
                cout << fileSize(substitute(substitute(path, "finalized", "blooms"),".bin",".bloom")) << ",";
                cout << (thing->hashes.operator[](start).hash) << ",";
                cout << (thing->blooms.operator[](start).hash) << "\n";

            }
        }
    }
    return !shouldQuit();
}
