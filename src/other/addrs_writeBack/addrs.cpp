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

extern bool visitFile(const string_q& path, void* data);
//----------------------------------------------------------------
int main(int argc, const char* argv[]) {
    nodeNotRequired();
    etherlib_init(quickQuitHandler);
    forEveryFileInFolder(indexFolder_finalized, visitFile, NULL);
    etherlib_cleanup();
}

//----------------------------------------------------------------
bool visitFile(const string_q& path, void* data) {
    if (endsWith(path, '/')) {
        return forEveryFileInFolder(path + "*", visitFile, data);

    } else {
        if (contains(path, ".bin")) {
            timestamp_t unused;
            blknum_t end;
            blknum_t begin =
            bnFromPath(path, end, unused);
            // if (begin < 5998901 || begin > 7000000) {
            if (begin < 800000 || begin > 1000000) {
                //                cout << "skipping " << path << endl;
                return true;
            }

            string_q outFile = substitute(substitute(path, indexFolder_finalized, "./sorted/"), ".bin", ".txt");
            string_q three = substitute(outFile, "./sorted/", "").substr(0, 3);
            outFile = substitute(outFile, "/sorted/", "/sorted/" + three + "/");
            establishFolder(outFile);
            if (fileExists(outFile)) {
                cout << "skipping " << outFile << endl;
                return true;
            }

            char* rawData = NULL;
            uint32_t nAddrs = 0;
            CArchive chunk(READING_ARCHIVE);
            if (!chunk.Lock(path, modeReadOnly, LOCK_NOWAIT)) {
                cerr << "Could not open index file " << path << ". Quitting...";
                return false;
            }

            size_t sz = fileSize(path);
            rawData = (char*)malloc(sz + (2 * 59));
            if (!rawData) {
                cerr << "Could not allocate memory for data. Quitting...";
                chunk.Release();
                return false;
            }
            bzero(rawData, sz + (2 * 59));
            size_t nRead = chunk.Read(rawData, sz, sizeof(char));
            if (nRead != sz) {
                cerr << "Could not read entire file. Quitting..." << endl;
                return true;
            }
            CHeaderRecord_base* h = (CHeaderRecord_base*)rawData;
            ASSERT(h->magic == MAGIC_NUMBER);
            ASSERT(bytes_2_Hash(h->hash) == versionHash);
            nAddrs = h->nAddrs;
            // uint32_t nRows = h->nRows; not used

            cout << endl << outFile << " (" << fileSize(path) << "-" << nAddrs << "): " << endl;

            CArchive output(WRITING_ARCHIVE);
            if (!output.Lock(outFile, modeWriteCreate, LOCK_NOWAIT)) {
                cout << "Could not lock file: " << outFile << endl;
                return true;
            }

            CAddressRecord_base* addrsOnFile = (CAddressRecord_base*)(rawData + sizeof(CHeaderRecord_base));
            CAppearance_base* blocksOnFile = (CAppearance_base*)&addrsOnFile[nAddrs];
            for (uint32_t a = 0; a < nAddrs; a++) {
                CAddressRecord_base* record = &addrsOnFile[a];
                for (size_t r = record->offset; r < record->offset + record->cnt; r++) {
                    CAppearance_base* app = &blocksOnFile[r];
                    ostringstream os;
                    os << bytes_2_Addr(record->bytes) << "\t" << padNum9(app->blk) << "\t" << padNum5(app->txid)
                    << endl;
                    output.WriteLine(os.str());
                }
                if (!(a % 1000))
                    cout << (a / 1000);
                if (!(a % 100))
                    cout << ".";
                cout.flush();
            }
            output.Release();
            ASSERT(rawData);
            delete rawData;
        }
    }
    return true;
}
