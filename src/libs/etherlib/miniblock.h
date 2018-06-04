#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks(tm) - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/

namespace qblocks {

    //--------------------------------------------------------------------------
    typedef enum {
        miniBlock=(0x1<<1),
        miniTrans=(0x1<<2),
        miniBlocks=(miniBlock|miniTrans)
    } miniBlockType;

    //------------------------------------------------------------------------
    class CMiniBlock {
    public:
        blknum_t    blockNumber;
        timestamp_t timestamp;
        txnum_t     firstTrans;
        txnum_t     nTrans;

                 CMiniBlock (void);
                 CMiniBlock (CBlock *block);
        bool     operator== (const CBlock& b) const;
        void     toBlock    (CBlock& block) const;
        SFString Format     (void) const;
    };

    //------------------------------------------------------------------------
    class CMiniTrans {
    public:
        uint32_t index;
        bool     isError;
        SFGas    gasUsed;
        SFGas    gasPrice;
        uint32_t nTraces;
        char     value[41];

                 CMiniTrans (void);
                 CMiniTrans (CTransaction *t);
        void     toTrans    (CTransaction& trans) const;
        SFString Format     (void) const;
    };

    //--------------------------------------------------------------------------
    class CInMemoryCache {
    public:
                 CInMemoryCache (void);
                ~CInMemoryCache (void);

        void     Init           (void);
        void     Clear          (void);

        bool     Load           (blknum_t _start, blknum_t _count);
        blknum_t firstBlock     (void);
        blknum_t lastBlock      (void);

    public:
        bool            isLoaded;

        CBinFile blocksOnDisc;
        CBinFile transOnDisc;

        // Very important note: the two pointers are not allocated. They are pointers to a location
        // in the memory mapped file. They appear to be allocated array pointers, but they are
        // actually FILE pointers and should not be cleaned up.
        CMiniBlock *blocks;
        const CMiniTrans *trans;

    private:
        blknum_t        nBlocks;
        txnum_t         nTrans;
        uint64_t        start;
        uint64_t        count;
    };

    //-------------------------------------------------------------------------
    // function pointer types for forEvery functions
    typedef bool (*MINIBLOCKVISITFUNC)(CMiniBlock& block, const CMiniTrans *trans, void *data);
    typedef bool (*MINITRANSVISITFUNC)(CMiniTrans& trans, void *data);
    typedef bool (*BLOCKVISITFUNC)(CBlock& block, void *data);

    //-------------------------------------------------------------------------
    extern bool forEveryFullBlockInMemory    (BLOCKVISITFUNC     func, void *data, uint64_t start, uint64_t count, uint64_t skip=1);
    extern bool forEveryMiniBlockInMemory    (MINIBLOCKVISITFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip=1);
    extern bool forOnlyMiniBlocks            (MINIBLOCKVISITFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip=1);
    extern bool forOnlyMiniTransactions      (MINITRANSVISITFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip=1);
    extern void clearInMemoryCache           (void);

}  // namespace qblocks

//-------------------------------------------------------------------------
extern bool visitMiniBlock(CMiniBlock& block, const CMiniTrans *trans, void *data);
