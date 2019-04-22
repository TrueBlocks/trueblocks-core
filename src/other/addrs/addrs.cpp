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

        CStringArray lines;
        if (!asciiExists || !binaryExists) {
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

        if (binaryExists) {
            cerr << "\t" << greenCheck << " Binary file " << binaryFn << " exists..." << endl;
        } else {
            writeIndexAsBinary(binaryFn, lines);
            // We now copy again to build the IPFS cache file. We zip the data first to make it as small as possible
            cerr << "\tCopying zip files..." << endl;
            string_q zipPath = substitute(binaryFn, indexFolder_finalized_v2, "./output-bzips/");
            string_q cmd = "cp -p " + binaryFn + " " + zipPath + " ; ";
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
THIS CAN BE REMOVED MOST LIKELY
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

extern void stats(const CNewBloomArray& blooms, const string_q& path);
extern bool visitFile(const string_q& path, void *data);
//----------------------------------------------------------------
int main(int argc, const char *argv[]) {
    nodeNotRequired();
    etherlib_init(defaultQuitHandler);
    forEveryFileInFolder(indexFolder_finalized_v2, visitFile, NULL);
    etherlib_cleanup();
}

//----------------------------------------------------------------
bool visitFile(const string_q& path, void *data) {

    if (endsWith(path, '/')) {
        return forEveryFileInFolder(path + "*", visitFile, data);

    } else {

        if (contains(path, ".bin")) {

            string_q outFile = substitute(substitute(path, "/finalized/", "/blooms/"), ".bin", ".bloom");
            ::remove((outFile+".lck").c_str());
            if (fileExists(outFile)) {
                cout << outFile << " exists" << endl;
                return !shouldQuit();
            }

            cout << path << ": " << fileSize(path) << endl;

            char *rawData = NULL;
            uint32_t nAddrs = 0;
            CArchive chunk(READING_ARCHIVE);
            if (!chunk.Lock(path, modeReadOnly, LOCK_NOWAIT)) {
                cerr << "Could not open index file " << path << ". Quitting...";
                return false;
            }

            size_t sz = fileSize(path);
            rawData = (char*)malloc(sz + (2*59));
            if (!rawData) {
                cerr << "Could not allocate memory for data. Quitting...";
                chunk.Release();
                return false;
            }
            bzero(rawData, sz + (2*59));
            size_t nRead = chunk.Read(rawData, sz, sizeof(char));
            if (nRead != sz) {
                cerr << "Could not read entire file. Quitting..." << endl;
                return !shouldQuit();
            }
            CHeaderRecord_base *h = (CHeaderRecord_base*)rawData;
            ASSERT(h->magic == MAGIC_NUMBER);
            ASSERT(bytes_2_Hash(h->hash) == versionHash);
            nAddrs = h->nAddrs;
            //uint32_t nRows = h->nRows; not used

            cout << nAddrs << " ";
            CNewBloomArray blooms;
            CAddressRecord_base *addrsOnFile = (CAddressRecord_base *)(rawData+sizeof(CHeaderRecord_base));
            for (uint32_t a = 0 ; a < nAddrs ; a++) {
                CAddressRecord_base *record = &addrsOnFile[a];
                addToSet(blooms, bytes_2_Addr(record->bytes));
                if (!(a%1000)) cout << (a/1000);
                if (!(a%100)) cout << ".";
                cout.flush();
            }

            lockSection(true);
            CArchive output(WRITING_ARCHIVE);
            if (!output.Lock(outFile, modeWriteCreate, LOCK_NOWAIT)) {
                lockSection(false);
                return false;
            }
            output.Write((uint32_t)blooms.size());
            for (auto bloom : blooms) {
                //bloom.showBloom(cout);
                output.Write((uint32_t)bloom.nInserted);
                output.Write(bloom.bits, sizeof(uint8_t), qblocks::bloom_nt::BYTE_SIZE);
            }
            output.Release();
            lockSection(false);

            stats(blooms, outFile);

            ASSERT(rawData);
            delete rawData;
        }
    }
    return !shouldQuit();
}

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
