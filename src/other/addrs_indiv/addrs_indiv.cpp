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
#include "pin lib.h"

typedef struct {
    CIndexHashMap hashes;
    CIndexHashMap blooms;
    string_q outPath;
} Thing;

//----------------------------------------------------------------
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);

    Thing thing;
    loadHashes(thing.hashes, "finalized");
    loadHashes(thing.blooms, "blooms");

    string_q indexPath = getIndexPath("finalized");
    thing.outPath = contains(indexPath, "JUNK") ? "./report.1/" : "./report.2/";

    forEveryFileInFolder(indexPath, visitFile, &thing);

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
            if (cnt++ > 13)
                return false;

            Thing *thing = (Thing*)data;

            blknum_t end;
            timestamp_t unused2;
            blknum_t start = bnFromPath(path, end, unused2);

            string_q outFile = thing->outPath + padNum9(start) + '-' + padNum9(end) + ".txt";
            cerr << "Outputing: " << outFile << endl;
            ofstream output;
            output.open(outFile, ios::out | ios::trunc);
            if (!output.is_open())
                return false;

            CIndexArchive index(READING_ARCHIVE);
            if (index.ReadIndexFromBinary(path)) {
                output << "start: " << start << endl;
                output << "end: " << end << endl;
                output << "fileSize: " << fileSize(path) << endl;
                output << "bloomSize: " << fileSize(substitute(substitute(path, "finalized", "blooms"),".bin",".bloom")) << endl;
                output << "indexHash: " << (thing->hashes.operator[](start).hash) << endl;
                output << "bllomHash: " << (thing->blooms.operator[](start).hash) << endl;
                output << "nAddrs: " << index.header->nAddrs << endl;
                output << "nRows: " << index.header->nRows << endl;
                for (uint32_t a = 0 ; a < index.nAddrs ; a++) {
                    CAddressRecord_base *aRec = &index.addresses[a];
                    //cout << "[" << a << "]: " << bytes_2_Addr(aRec->bytes) << "\t" << aRec->offset << "\t" << aRec->cnt << endl;
                    output << bytes_2_Addr(aRec->bytes) << "\t" << aRec->offset << "\t" << aRec->cnt << endl;
                }
                for (uint32_t a = 0 ; a < index.nApps ; a++) {
                    CAppearance_base *aRec = &index.appearances[a];
                    output << aRec->blk << "\t" << aRec->txid << endl;
                }
                output.close();
            }
        }
    }
    return !shouldQuit();
}
