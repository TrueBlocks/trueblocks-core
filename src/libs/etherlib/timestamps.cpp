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
bool freshenAndLoad(blknum_t minBlk) {
    if (!isTestMode() && !isApiMode())
        freshenTimestamps(minBlk);  // opens, freshens, and closes the file
    if (!loadTimestamps(&expContext().tsMemMap, expContext().tsCnt)) {
        LOG_WARN("Could not load timestamp file");
        return false;
    }
    LOG4("Loaded timestamp file");
    return true;
}

//-------------------------------------------------------------------------
blknum_t getTimestampBlockAt(blknum_t blk) {
    if (!expContext().tsMemMap)
        if (!freshenAndLoad(blk))
            return 0;
    if (expContext().tsMemMap && blk < expContext().tsCnt)
        return expContext().tsMemMap[(blk * 2)];
    return 0;
}

//-------------------------------------------------------------------------
timestamp_t getTimestampAt(blknum_t blk) {
    if (!expContext().tsMemMap)
        if (!freshenAndLoad(blk))
            return 0;
    if (expContext().tsMemMap && blk < expContext().tsCnt)
        return timestamp_t(expContext().tsMemMap[(blk * 2) + 1]);
    return 0;
}

//-------------------------------------------------------------------------
size_t nTimestamps(void) {
    return ((fileSize(indexFolderBin_ts) / sizeof(uint32_t)) / 2);
}

//-----------------------------------------------------------------------
bool establishTsFile(void) {
    if (fileExists(indexFolderBin_ts))
        return true;

    establishFolder(indexFolder);

    time_q zipDate = fileLastModifyDate(chainConfigsZip_ts);
    time_q tsDate = fileLastModifyDate(indexFolderBin_ts);

    if (zipDate > tsDate) {
        ostringstream cmd;
        cmd << "cd \"" << indexFolder << "\" ; ";
        cmd << "cp \"" << chainConfigsZip_ts << "\" . ; ";
        cmd << "gunzip ts.bin.gz";
        string_q result = doCommand(cmd.str());
        // The original zip file still exists
        ASSERT(fileExists(chainConfigsZip_ts));
        // The new timestamp file exists
        ASSERT(fileExists(indexFolderBin_ts));
        // The copy of the zip file does not exist
        ASSERT(!fileExists(indexFolderBin_ts + ".gz"));
        return fileExists(indexFolderBin_ts);
    }

    // starts at zero...
    return true;
}

//-----------------------------------------------------------------------
bool freshenTimestamps(blknum_t minBlock) {
    if (isTestMode())
        return true;

    // LOG_INFO("Not test mode. minBlock: ", minBlock);
    if (!establishTsFile())
        return false;

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
            ostringstream post;
            post << " (" << ((minBlock + 1) - block.blockNumber);
            post << " " << block.timestamp << " - " << ts_2_Date(block.timestamp).Format(FMT_EXPORT) << ")";
            post << "\r";
            LOG_PROGRESS(UPDATE, block.blockNumber, minBlock, post.str());
        }
    }
    LOG_PROGRESS(COMPLETE, block.blockNumber, getMetaData().client, string_q(80, ' '));

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

    if (!establishTsFile())
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

//-------------------------------------------------------------------------
bool forEveryTimestamp(BLOCKVISITFUNC func, void* data) {
    if (!func)
        return false;

    size_t n = nTimestamps();
    for (size_t index = 0; index < n; index++) {
        CBlock block;
        block.blockNumber = getTimestampBlockAt(index);
        block.timestamp = getTimestampAt(index);
        bool ret = (*func)(block, data);
        if (!ret)
            return false;
    }
    return true;
}

}  // namespace qblocks
