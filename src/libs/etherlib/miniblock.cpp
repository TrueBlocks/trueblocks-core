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
#include <algorithm>
#include "node.h"
#include "miniblock.h"

namespace qblocks {

    //--------------------------------------------------------------------------
    CMiniBlock::CMiniBlock(void) {
        bzero(this, sizeof(CMiniBlock));
    }

    //--------------------------------------------------------------------------
    CMiniBlock::CMiniBlock(const CBlock *block) {
        bzero(this, sizeof(CMiniBlock));
        blockNumber = block->blockNumber;
        timestamp   = block->timestamp;
        firstTrans  = 0;
        nTrans      = 0;
    }

    //--------------------------------------------------------------------------
    void CMiniBlock::toBlock(CBlock& block) const {
        block.blockNumber = blockNumber;
        block.timestamp   = timestamp;
        return;
    }

    //--------------------------------------------------------------------------
    string_q CMiniBlock::Format(void) const {
        ostringstream os;
        os << "blockNumber: " << blockNumber << " ";
        os << "timestamp: "   << timestamp   << " ";
        os << "firstTrans: "  << firstTrans  << " ";
        os << "nTrans: "      << nTrans      << " ";
        return os.str().c_str();
    }

    //--------------------------------------------------------------------------
    bool CMiniBlock::operator==(const CBlock& b) const {
        if (b.blockNumber         != blockNumber) return false;
        if (b.timestamp           != timestamp) return false;
        if (b.transactions.size() != nTrans) return false;
        return true;
    }

    //--------------------------------------------------------------------------
    CMiniTrans::CMiniTrans(void) {
        bzero(this, sizeof(CMiniTrans));
    }

    //--------------------------------------------------------------------------
    CMiniTrans::CMiniTrans(const CTransaction *t) {
        bzero(this, sizeof(CMiniTrans));
        index    = (uint32_t)t->transactionIndex;
        gasUsed  = t->receipt.gasUsed;
        gasPrice = t->gasPrice;
        isError  = t->isError;
        nTraces  = (uint32_t)getTraceCount(t->hash);
        strncpy(value, wei_2_Str(t->value).c_str(), 40); value[40] = '\0';
    }

    //--------------------------------------------------------------------------
    void CMiniTrans::toTrans(CTransaction& trans) const {
        trans.transactionIndex = index;
        trans.receipt.gasUsed  = gasUsed;
        trans.gasPrice         = gasPrice;
        trans.isError          = isError;
        trans.value            = str_2_Wei(value);
        return;
    }

    //--------------------------------------------------------------------------
    string_q CMiniTrans::Format(void) const {
        ostringstream os;
        os << "index: "    << index      << " ";
        os << "isError: "  << isError    << " ";
        os << "nTraces: "  << nTraces    << " ";
        os << "gasUsed: "  << gasUsed    << " ";
        os << "gasPrice: " << gasPrice   << " ";
        os << "value: "    << value      << " ";
        return os.str().c_str();
    }

    //--------------------------------------------------------------------------
    CInMemoryCache::CInMemoryCache(void) {
        Init();
    }

    //--------------------------------------------------------------------------
    CInMemoryCache::~CInMemoryCache(void) {
        Clear();
    }

    //--------------------------------------------------------------------------
    void CInMemoryCache::Init(void) {
        // blocksOnDisc
        // transOnDisc
        blocks    = NULL;
        trans     = NULL;

        nBlocks   = fileSize(miniBlockCache) / sizeof(CMiniBlock);
        nTrans    = fileSize(miniTransCache) / sizeof(CMiniTrans);
        start     = 0;
        count     = 0;

        isLoaded  = false;
    }

    //--------------------------------------------------------------------------
    void CInMemoryCache::Clear(void) {
        // VERY IMPORTANT NOTE: The 'block' and 'trans' pointers are not allocated, they are FILE pointers
        // instead, so don't 'delete' them.
        // if (blocks)
        //     delete [] blocks;
        // if (trans)
        //     delete [] trans;
        blocks = NULL;
        trans = NULL;

        blocksOnDisc.Release();
        transOnDisc.Release();

        Init();
    }

    //--------------------------------------------------------------------------
    inline string_q TIMER_IN(double& st) {
        ostringstream os;
        os << (qbNow()-st) << ": ";
        st = qbNow();
        return os.str().c_str();
    }

    //-------------------------------------------------------------------------
    inline string_q TIMER_TICK(double st) {
        ostringstream os;
        os << "in " << cGreen << (qbNow()-st) << cOff << " seconds.";
        return os.str().c_str();
    }

    //--------------------------------------------------------------------------
    bool CInMemoryCache::Load(blknum_t _start, blknum_t _count) {

        double startTime = qbNow();

        //--------------------------------------------------------------------------
        // Only come through here once, even if we fail to load
        if (isLoaded)
            return true;
        isLoaded = true;

        // Cannot process anything if the files are locked.
        if (isFileLocked(miniBlockCache)) {
            cerr << "The miniBlockCache (" << miniBlockCache << ") is locked. The program cannot be run. Quitting...\n";
            cerr.flush();
            exit(0);
        } else if (isFileLocked(miniTransCache)) {
            cerr << "The miniTransCache (" << miniTransCache << ") is locked. The program cannot be run. Quitting...\n";
            cerr.flush();
            exit(0);
        }

        blknum_t latestBlock = getLatestBlockFromCache();
        start = min(_start,          latestBlock);
        count = min(_start + _count, latestBlock) - _start;

        CMemMapFile blockFile(miniBlockCache.c_str(),  CMemMapFile::WholeFile, CMemMapFile::SequentialScan);
        blocks = (CMiniBlock*)(blockFile.getData());  // NOLINT

        CMemMapFile transFile(miniTransCache.c_str(),  CMemMapFile::WholeFile, CMemMapFile::SequentialScan);
        trans  = reinterpret_cast<const CMiniTrans *>(transFile.getData());

        cerr << "\n" << TIMER_IN(startTime);
        return true;
    }

    //--------------------------------------------------------------------------
    blknum_t CInMemoryCache::firstBlock(void) {
        return 0;
    }

    //--------------------------------------------------------------------------
    blknum_t CInMemoryCache::lastBlock(void) {
        return nBlocks;
    }

    //--------------------------------------------------------------------------
    static CInMemoryCache *theCache = NULL;

    //--------------------------------------------------------------------------
    void clearInMemoryCache(void) {
        if (theCache)
            theCache->Clear();
        delete theCache;
        theCache = NULL;
    }

    //--------------------------------------------------------------------------
    CInMemoryCache *getTheCache(void) {
        if (!theCache)
            theCache = new CInMemoryCache();
        return theCache;
    }

    //--------------------------------------------------------------------------
    bool forEveryMiniBlockInMemory(MINIBLOCKVISITFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip) {

        CInMemoryCache *cache = getTheCache();
        if (!cache->Load(start, count))
            return false;

        blknum_t first = cache->firstBlock();
        blknum_t last = cache->lastBlock();

        bool done = false;
        for (blknum_t i = first ; i < last && !done ; i = i + skip) {

            if (inRange(cache->blocks[i].blockNumber, start, start + count - 1)) {

                if (!(*func)(cache->blocks[i], &cache->trans[0], data))
                    return false;

            } else if (cache->blocks[i].blockNumber >= start + count) {

                done = true;

            } else {

                // do nothing
            }
        }

        return true;
    }

    //--------------------------------------------------------------------------
    bool forEveryFullBlockInMemory(BLOCKVISITFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip) {

        CInMemoryCache *cache = getTheCache();
        if (!cache->Load(start, count))
            return false;

        blknum_t first = cache->firstBlock();
        blknum_t last  = cache->lastBlock();

        bool done = false;
        for (blknum_t i = first ; i < last && !done ; i = i + skip) {

            if (inRange(cache->blocks[i].blockNumber, start, start + count - 1)) {

                CBlock block;
                cache->blocks[i].toBlock(block);
                gas_t gasUsed = 0;
                for (txnum_t tr = cache->blocks[i].firstTrans ;
                        tr < cache->blocks[i].firstTrans + cache->blocks[i].nTrans;
                        tr++) {
                    CTransaction tt;
                    cache->trans[tr].toTrans(tt);
                    gasUsed += tt.receipt.gasUsed;
                    block.transactions.push_back(tt);
                }
                block.gasUsed = gasUsed;
                if (!(*func)(block, data))
                    return false;

            } else if (cache->blocks[i].blockNumber >= start + count) {

                done = true;

            } else {

                // do nothing
            }
        }

        return true;
    }

    bool forOnlyMiniBlocks(MINIBLOCKVISITFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip) {
        return true;
    }

    bool forOnlyMiniTransactions(MINITRANSVISITFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip) {
        return true;
    }

}  // namespace qblocks
