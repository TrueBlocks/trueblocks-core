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
#include "etherlib.h"
#include "options.h"

//--------------------------------------------------------------------------
bool markFullBlocks(const string_q& path, void *data) {

    if (endsWith(path, "/")) {
        forAllFiles(path + "*", markFullBlocks, data);

    } else {

        if (contains(path, ".bin")) {
            CArchive *pRes = (CArchive*)data;
            ASSERT(pRes && pRes->isOpen());

            cout << "\tMarking " << path << "              \r";
            lockSection(true);
            pRes->Write(bnFromPath(path));
            pRes->flush();
            lockSection(false);

        } else {
            cout << "\tSkipping " << path << "              \r";

        }
        cout.flush();
    }

    // return true if we should continue (i.e. we should not quit)
    return !shouldQuit();
}

//--------------------------------------------------------------------------
bool establishFullBlockIndex(void) {

    // If the full block index already exists, we don't need to re-create it
    string_q finFile = substitute(fullBlockIndex, ".bin", ".fin");
    if (fileExists(finFile) && fileSize(fullBlockIndex) > 0)
        return true;

    cerr << bGreen << "Rebuilding full block index...\n" << cOff;
    cerr.flush();

    // Otherwise we rebuild it from scratch by visiting each binary block
    CArchive fullBlockCache(WRITING_ARCHIVE);
    if (fullBlockCache.Lock(fullBlockIndex, binaryWriteCreate, LOCK_WAIT)) {
        lockSection(true);
        ASSERT(fullBlockCache.isOpen());
        bool finished = forEveryFileInFolder(bloomFolder, markFullBlocks, &fullBlockCache);
        fullBlockCache.Release();
        lockSection(false);
        if (finished) {
            cerr << bGreen << "Re-indexing completed.\n" << cOff;
            stringToAsciiFile(finFile, "Removing this file will cause the index to rebuild\n");
        } else {
            cerr << bRed << "Re-indexing aborted, will re-run next time blockScrape is started.\n" << cOff;
        }
        cerr.flush();
        return finished;
    }

    cerr << "establishFullBlockIndex failed: " << fullBlockCache.LockFailure() << "\n";
    return false;
}

//--------------------------------------------------------------------------------
bool updateIndex(CArchive& fullBlockCache, blknum_t bn) {

    ASSERT(fullBlockCache.isOpen());
    fullBlockCache.Write(bn);
    fullBlockCache.flush();

    return true;
}
