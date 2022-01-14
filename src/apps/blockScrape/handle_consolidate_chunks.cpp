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
#include "consolidator.h"

//--------------------------------------------------------------------------
bool CConsolidator::consolidate_chunks(void) {
    ENTER("consolidate_chunks");

    // We are now in a valid state. All records that have not yet been consolidated
    // are in newStage. Count how many lines we have...
    blknum_t nRecords = fileSize(newStage) / 59;
    blknum_t chunkSize = opts->apps_per_chunk;
    int64_t distToHead = (int64_t(chunkSize) - int64_t(nRecords));

    LOG_FN8(tmpFile);
    LOG_FN8(tmp_fn);
    LOG_FN8(oldStage);
    LOG_FN8(newStage);
    LOG8("nRecords: ", nRecords);
    LOG8("chunkSize: ", chunkSize);
    LOG8("Distance to head: ", distToHead);

    // ...if we don't have enough, return and get more...
    if (nRecords <= chunkSize) {
        LOG_INFO("");
        LOG_INFO(bBlue, "Consolidation not ready...", cOff);
        LOG_INFO(cYellow, "  Have ", nRecords, " records of ", chunkSize, ". Need ", distToHead, " more.", cOff);
        LOG_FN8(newStage);
        EXIT_NOMSG(true);
    }

    EXIT_NOMSG(write_chunks(chunkSize, false /* atLeastOnce */));
}

//---------------------------------------------------------------------------------------------------
bool visitToPin(const string_q& chunkId, void* data) {
    LOG_INFO("  Pinning");
    LOG_FN8(chunkId);
    ASSERT(data);
    CPinnedChunkArray& pinList = *(CPinnedChunkArray*)data;  // NO_LINT
    CPinnedChunk pinRecord;
    pinlib_pinChunk(pinList, chunkId, pinRecord);
    string_q ci = substitute(pinRecord.fileName, indexFolder_finalized, "");
    ci = substitute(ci, indexFolder_blooms, "");
    ci = substitute(ci, ".bin", "");
    ostringstream os;
    os << ci << "\t" << pinRecord.bloomHash << "\t" << pinRecord.indexHash << endl;
    string_q manifestFile = chainFolderTxt_manifest;
    os << asciiFileToString(manifestFile);
    stringToAsciiFile(manifestFile, os.str());
    return !shouldQuit();
}
