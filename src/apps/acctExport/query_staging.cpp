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

//---------------------------------------------------------------
bool visitStagingIndexFiles(const string_q& path, void* data) {
    if (endsWith(path, "/")) {
        return forEveryFileInFolder(path + "*", visitStagingIndexFiles, data);

    } else {
        // There should only be a single file in the ./staging folder (other than
        // 0000000000-temp.txt which we skip). The file contains all blocks the scraper
        // has seen but has not yet consolidated. Here, we read the file which is
        // fixed length: [address, blockNum, txid ('asciiAppearanceSize' bytes)]. We read the entire file
        // into memory at once, sort it by address (the file is sorted by block and since
        // new blocks will be appended, there's no reason so sort it each time) and then
        // binary search for the address we're looking for. Should be super fast.
        COptions* options = reinterpret_cast<COptions*>(data);
        options->stats.nFiles++;

        if (!contains(path, "staging/0") || !endsWith(path, ".txt") || contains(path, "-temp")) {
            options->stats.nSkipped++;
            return !shouldQuit();
        }

        timestamp_t unused;
        options->fileRange.first = path_2_Bn(path, options->fileRange.second, unused);
        ASSERT(unused != NOPOS && options->fileRange.first != NOPOS && options->fileRange.second != NOPOS);

        // Note that `start` and `end` options are ignored when scanning
        if (!rangesIntersect(options->listRange, options->fileRange)) {
            options->stats.nSkipped++;
            return !shouldQuit();
        }

        options->stats.nStageChecked++;

        return options->queryFlatFile(path, true /* sorted */);
    }
    ASSERT(0);  // should not happen
    return !shouldQuit();
}
