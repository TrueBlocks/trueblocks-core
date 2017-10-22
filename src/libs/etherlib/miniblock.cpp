/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "node.h"
#include "miniblock.h"

namespace qblocks {

    //--------------------------------------------------------------------------
    CMiniBlock::CMiniBlock(void) {
        bzero(this, sizeof(CMiniBlock));
    }

    //--------------------------------------------------------------------------
    CMiniBlock::CMiniBlock(CBlock *block) {
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
    SFString CMiniBlock::Format(void) const {
        CStringExportContext ctx;
        ctx << "blockNumber: " << blockNumber << " ";
        ctx << "timestamp: "   << timestamp   << " ";
        ctx << "firstTrans: "  << firstTrans  << " ";
        ctx << "nTrans: "      << nTrans      << " ";
        return ctx.str;
    }

    //--------------------------------------------------------------------------
    bool CMiniBlock::operator==(const CBlock& b) const {
        if (b.blockNumber             != blockNumber) return false;
        if (b.timestamp               != timestamp) return false;
        if (b.transactions.getCount() != nTrans) return false;
        return true;
    }

    //--------------------------------------------------------------------------
    CMiniTrans::CMiniTrans(void) {
        bzero(this, sizeof(CMiniTrans));
    }

    //--------------------------------------------------------------------------
    CMiniTrans::CMiniTrans(CTransaction *t) {
        bzero(this, sizeof(CMiniTrans));
        index      = (uint32_t)t->transactionIndex;
        gasUsed    = t->receipt.gasUsed;
        gasPrice   = t->gasPrice;
        gasAllowed = t->gas;
        isError    = t->isError;
        strncpy(from,  fromAddress(t->from).substr(2), 40); from [40] = '\0';
        strncpy(to,    fromAddress(t->to).substr(2),   40); to   [40] = '\0';
        strncpy(value, fromWei    (t->value),          40); value[40] = '\0';
    }

    //--------------------------------------------------------------------------
    void CMiniTrans::toTrans(CTransaction& trans) const {
        trans.transactionIndex = index;
        trans.gas              = gasAllowed;
        trans.receipt.gasUsed  = gasUsed;
        trans.gasPrice         = gasPrice;
        trans.isError          = isError;
        trans.from             = toAddress(from);
        trans.to               = toAddress(to);
        trans.value            = toWei(value);
        return;
    }

    //--------------------------------------------------------------------------
    SFString CMiniTrans::Format(void) const {
        CStringExportContext ctx;
        ctx << "index: "    << index      << " ";
        ctx << "from: "     << from       << " ";
        ctx << "to: "       << to         << " ";
        ctx << "value: "    << value      << " ";
        ctx << "isError: "  << isError    << " ";
        ctx << "gas: "      << gasAllowed << " ";
        ctx << "gasUsed: "  << gasUsed    << " ";
        ctx << "gasPrice: " << gasPrice   << " ";
        return ctx.str;
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

        blockFile = miniBlockCache;
        transFile = miniTransCache;

        nBlocks   = fileSize(blockFile) / sizeof(CMiniBlock);
        nTrans    = fileSize(transFile) / sizeof(CMiniTrans);
        start     = 0;
        count     = 0;

        isLoaded  = false;
    }

    //--------------------------------------------------------------------------
    void CInMemoryCache::Clear(void) {
        if (blocks)
            delete [] blocks;
        blocks = NULL;

        if (trans)
            delete [] trans;
        trans = NULL;

        blocksOnDisc.Release();
        transOnDisc.Release();

        Init();
    }

    //--------------------------------------------------------------------------
    bool CInMemoryCache::Load(blknum_t _start, blknum_t _count) {
        blknum_t latestBlock = getLatestBlockFromCache();

        start = min(_start,          latestBlock);
        count = min(_start + _count, latestBlock) - _start;
        if (isLoaded)
            return true;
        isLoaded = true; // only come through here once, even if we fail to load

        double startTime = qbNow();
        blocks = new CMiniBlock[nBlocks];
        if (!blocks) {
            cerr << "Could not allocate memory for the blocks (size needed: " << nBlocks << ").\n";
            return false;
        }
        bzero(blocks, sizeof(CMiniBlock)*(nBlocks));
        if (verbose)
            cerr << TIMER_IN(startTime) << "Allocated room for " << nBlocks << " miniBlocks.\n";

        // Next, we try to open the mini-block database
        if (!blocksOnDisc.Lock(blockFile, binaryReadOnly, LOCK_WAIT)) {
            cerr << "Could not open the mini-block database: " << blockFile << ".\n";
            return false;
        }
        blocksOnDisc.Seek(0, SEEK_SET);

        // Read the entire mini-block database into memory in one chunk
        size_t nRead = blocksOnDisc.Read(blocks, nBlocks, sizeof(CMiniBlock));
        blocksOnDisc.Release();  // We're done with it
        if (nRead != nBlocks) {
            cerr << "Error encountered reading mini-blocks database.\n Quitting...";
            return false;
        }
        if (verbose)
            cerr << TIMER_IN(startTime) << "Read " << nRead << " miniBlocks into memory.\n";

        // See if we can allocation enough space for the mini-transaction database
        SFUint32 fs = fileSize(transFile);
        SFUint32 ms = sizeof(CMiniTrans);
        nTrans   = fs / ms;
        trans = new CMiniTrans[nTrans];
        if (!trans) {
            cerr << "Could not allocate memory for the transactions (size needed: " << nTrans << ").\n";
            return false;
        }
        bzero(trans, sizeof(CMiniTrans)*(nTrans));
        if (verbose)
            cerr << TIMER_IN(startTime) << "Allocated room for " << nTrans << " transactions.\n";

        // Next, we try to open the mini-transaction database
        if (!transOnDisc.Lock(transFile, binaryReadOnly, LOCK_WAIT)) {
            cerr << "Could not open the mini-transaction database: " << transFile << ".\n";
            return false;
        }

        // Read the entire mini-transaction database into memory in one chunk
        // TODO: What is the correct value for this?
#define READ_SIZE 204800
        nRead = 0;
        while (nRead < nTrans) {
            nRead += transOnDisc.Read(&trans[nRead], READ_SIZE, sizeof(CMiniTrans));
            if (verbose)
                progressBar(nRead, nTrans, qbNow() - startTime);
        }
        transOnDisc.Release();

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
        theCache = NULL;
    }
    //--------------------------------------------------------------------------
    CInMemoryCache *getTheCache(void) {
        if (!theCache)
            theCache = new CInMemoryCache();
        return theCache;
    }

    //--------------------------------------------------------------------------
    bool forEveryMiniBlockInMemory(MINIBLOCKVISITFUNC func, void *data, blknum_t start, blknum_t count) {

        CInMemoryCache *cache = getTheCache();
        if (!cache->Load(start,count))
            return false;

        blknum_t first = cache->firstBlock();
        blknum_t last = cache->lastBlock();

        bool done = false;
        for (blknum_t i = first ; i < last && !done ; i++) {

            if (inRange(cache->blocks[i].blockNumber, start, start+count-1)) {

                if (!(*func)(cache->blocks[i], &cache->trans[0], data))
                    return false;

            } else if (cache->blocks[i].blockNumber >= start+count) {

                done = true;

            } else {

                // do nothing
            }
        }

        return true;
    }

    //--------------------------------------------------------------------------
    bool forEveryFullBlockInMemory(BLOCKVISITFUNC func, void *data, blknum_t start, blknum_t count) {

        CInMemoryCache *cache = getTheCache();
        if (getSource() != "mem")
            return false;
        if (!cache->Load(start, count))
            return false;

        blknum_t first = cache->firstBlock();
        blknum_t last  = cache->lastBlock();

        bool done=false;
        for (blknum_t i = first ; i < last && !done ; i++) {

            if (inRange(cache->blocks[i].blockNumber, start, start+count-1)) {

                CBlock block;
                cache->blocks[i].toBlock(block);
                SFGas gasUsed = 0;
                for (txnum_t tr = cache->blocks[i].firstTrans ; tr < cache->blocks[i].firstTrans + cache->blocks[i].nTrans ; tr++) {
                    CTransaction tt;
                    cache->trans[tr].toTrans(tt);
                    gasUsed += tt.receipt.gasUsed;
                    block.transactions[block.transactions.getCount()] = tt;
                }
                block.gasUsed = gasUsed;
                if (!(*func)(block, data))
                    return false;

            } else if (cache->blocks[i].blockNumber >= start+count) {

                done=true;

            } else {

                // do nothing
            }
        }

        return true;
    }

    bool forOnlyMiniBlocks(MINIBLOCKVISITFUNC func, void *data, blknum_t start, blknum_t count) {
        return true;
    }

    bool forOnlyMiniTransactions(MINITRANSVISITFUNC func, void *data, blknum_t start, blknum_t count) {
        return true;
    }

}  // namespace qblocks
