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
#include "node.h"

namespace qblocks {

//-------------------------------------------------------------------------
timestamp_t bn_2_Timestamp(blknum_t blk) {
    if (!expContext().tsMemMap) {
        if (!isTestMode() && !isApiMode()) {
            freshenTimestamps(blk);
        }

        if (!loadTimestamps(&expContext().tsMemMap, expContext().tsCnt)) {
            LOG_WARN("Could not load timestamp file");
            return 0;
        }
    }

    if (expContext().tsMemMap && blk < expContext().tsCnt) {
        return timestamp_t(expContext().tsMemMap[(blk * 2) + 1]);
    }

    return 0;
}

//-----------------------------------------------------------------------
bool freshenTimestamps(blknum_t minBlock) {
    if (isTestMode())
        return true;

    // LOG_INFO("Established ts file");
    size_t nRecords = ((fileSize(indexFolderBin_ts) / sizeof(uint32_t)) / 2);
    if (nRecords >= minBlock)
        return true;

    if (fileExists(indexFolderBin_ts + ".lck")) {  // it's being updated elsewhere
        LOG_ERR("Timestamp file ", indexFolderBin_ts, " is locked. Cannot update. Re-running...");
        ::remove((indexFolderBin_ts + ".lck").c_str());
        return false;
    }

    lockSection();

    // LOG_INFO("Updating");
    CArchive file(WRITING_ARCHIVE);
    if (!file.Lock(indexFolderBin_ts, modeWriteAppend, LOCK_NOWAIT)) {
        LOG_ERR("Failed to open ", indexFolderBin_ts);
        unlockSection();
        return false;
    }

    if (nRecords == 0) {
        file << (uint32_t)0 << (uint32_t)blockZeroTs();
        file.flush();
        LOG_INFO(padNum9(0), "\t", blockZeroTs(), "\t", ts_2_Date(blockZeroTs()).Format(FMT_EXPORT), "          \r");
        nRecords++;
    }

    CBlock block;
    for (blknum_t bn = nRecords; bn < (minBlock + 1) && !shouldQuit(); bn++) {
        block = CBlock();  // reset
        getBlockHeader(block, bn);
        if (block.timestamp < blockZeroTs()) {
            LOG_ERR("Bad data when requesting block '", bn, "'. ", block.Format("[{BLOCKNUMBER}].[{TIMESTAMP}]"),
                    " Cannot continue.");
            file.Release();
            unlockSection();
            return false;
        } else {
            file << ((uint32_t)block.blockNumber) << ((uint32_t)block.timestamp);
            file.flush();
            if (!isTestMode()) {
                ostringstream post;
                post << padRight("Update", 11) << " " << padNum8T(block.blockNumber) << " of " << padNum8T(minBlock)
                     << " (" << ((minBlock + 1) - block.blockNumber);
                post << " " << block.timestamp << " - " << ts_2_Date(block.timestamp).Format(FMT_EXPORT) << ")";
                post << "             \r";
                LOG_PROG(post.str());
            }
        }
    }
    // LO G_PROGRESS(COMPLETE, block.blockNumber, minBlock, IGNORE_BLOCK, string_q(80, ' '));

    file.Release();
    unlockSection();

    return true;
}

//-----------------------------------------------------------------------
bool loadTimestamps(uint32_t** theArray, size_t& cnt) {
    static CMemMapFile file;
    if (file.is_open())
        file.close();
    if (cnt == size_t(NOPOS))
        return false;

    // Order matters.
    // User may call us with a NULL array pointer, but we still want to file 'cnt'
    cnt = ((fileSize(indexFolderBin_ts) / sizeof(uint32_t)) / 2);
    if (theArray == NULL)
        return cnt;

    file.open(indexFolderBin_ts);
    if (file.isValid())
        *theArray = (uint32_t*)file.getData();  // NOLINT

    return true;
}

//-------------------------------------------------------------------------
bool correctTimestamp(blknum_t blk, timestamp_t ts) {
    size_t nRecords = ((fileSize(indexFolderBin_ts) / sizeof(uint32_t)) / 2);

    // If we're in test mode, the timestamps file doesn't exist, or the block number is too damn high, quit
    if (isTestMode() || !fileExists(indexFolderBin_ts) || blk >= nRecords)
        return true;

    expContext().tsCnt = size_t(NOPOS);
    loadTimestamps(&expContext().tsMemMap, expContext().tsCnt);

    CArchive file(WRITING_ARCHIVE);
    if (!file.Lock(indexFolderBin_ts, modeReadWrite, LOCK_WAIT)) {
        LOG_ERR("Failed to open ", indexFolderBin_ts);
        return false;
    }

    uint32_t* buffer = new uint32_t[nRecords * 2];
    if (!buffer) {
        LOG_ERR("Could not allocate memory for timestamps");
        file.Release();
        return false;
    }

    if (!file.Read(buffer, sizeof(uint32_t), nRecords * 2)) {
        LOG_ERR("Could not read timestamp file ", indexFolderBin_ts);
        delete[] buffer;
        file.Release();
        return false;
    }

    buffer[blk * 2] = uint32_t(blk);
    buffer[blk * 2 + 1] = uint32_t(ts);
    lockSection();
    file.Seek(0, SEEK_SET);
    file.Write(buffer, sizeof(uint32_t), nRecords * 2);
    unlockSection();
    file.Release();
    delete[] buffer;

    return true;
}

}  // namespace qblocks
