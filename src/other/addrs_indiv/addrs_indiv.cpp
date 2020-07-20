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
    CIndexHashMap blooms;
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

uint32_t cnt = 0;
//----------------------------------------------------------------
bool visitFile(const string_q& path, void* data) {
    if (endsWith(path, '/')) {
        forEveryFileInFolder(path + "*", visitFile, data);
    } else {
        if (endsWith(path, ".bin")) {
            if (cnt++ > 1)
                return false;
            blknum_t end;
            timestamp_t unused2;
            blknum_t start = bnFromPath(path, end, unused2);
            Thing *thing = (Thing*)data;
            CIndexArchive index(READING_ARCHIVE);
            if (index.ReadIndexFromBinary(path)) {
                cout << "start: " << start << endl;
                cout << "end: " << end << endl;
                cout << "nAddrs: " << index.header->nAddrs << endl;
                cout << "nRows: " << index.header->nRows << endl;
                bool stop = false;
                for (uint32_t a = 0 ; a < index.nAddrs && !stop ; a++) {
                    CAddressRecord_base *aRec = &index.addresses[a];
                    //cout << "[" << a << "]: " << bytes_2_Addr(aRec->bytes) << "\t" << aRec->offset << "\t" << aRec->cnt << endl;
                    cout << bytes_2_Addr(aRec->bytes) << "\t" << aRec->offset << "\t" << aRec->cnt << endl;
                    stop = aRec->offset > 9479;
                }
                for (uint32_t a = 0 ; a < index.nApps ; a++) {
                    CAppearance_base *aRec = &index.appearances[a];
                    cout << aRec->blk << "\t" << aRec->txid << endl;
                }
                cout << "fileSize: " << fileSize(path) << endl;
                cout << "bloomSize: " << fileSize(substitute(substitute(path, "finalized", "blooms"),".bin",".bloom")) << endl;
                cout << "indexHash: " << (thing->hashes.operator[](start).hash) << endl;
                cout << "bllomHash: " << (thing->blooms.operator[](start).hash) << endl;

            }
        }
    }
    return !shouldQuit();
}
