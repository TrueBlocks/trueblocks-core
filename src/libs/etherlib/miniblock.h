#pragma once
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

#define MINI_TRANS 1
namespace qblocks {

    //------------------------------------------------------------------------
    class CMiniBlock {
    public:
        blknum_t    blockNumber;
        timestamp_t timestamp;
        txnum_t     firstTrans;
#ifdef MINI_TRANS
        txnum_t     nTrans;
#endif

                 CMiniBlock (void);
        explicit CMiniBlock (const CBlock *block);
        bool     operator== (const CBlock& b) const;
        void     toBlock    (CBlock& block) const;
        string_q Format     (void) const;
    };

#ifdef MINI_TRANS
    //------------------------------------------------------------------------
    class CMiniTrans {
    public:
        uint32_t index;
        bool     isError;
        gas_t    gasUsed;
        gas_t    gasPrice;
        uint32_t nTraces;
        char     value[41];

                 CMiniTrans (void);
        explicit CMiniTrans (const CTransaction *t);
        void     toTrans    (CTransaction& trans) const;
        string_q Format     (void) const;
    };
#endif

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
#ifdef MINI_TRANS
        CBinFile transOnDisc;
#endif
        // Very important note: the two pointers are not allocated. They are pointers to a location
        // in the memory mapped file. They appear to be allocated array pointers, but they are
        // actually FILE pointers and should not be cleaned up.
        CMiniBlock *blocks;
#ifdef MINI_TRANS
        const CMiniTrans *trans;
#endif

    private:
        blknum_t        nBlocks;
#ifdef MINI_TRANS
        txnum_t         nTrans;
#endif
        uint64_t        start;
        uint64_t        count;
    };

    //-------------------------------------------------------------------------
    // function pointer types for forEvery functions
#ifdef MINI_TRANS
    typedef bool (*MINIBLOCKVISITFUNC)(CMiniBlock& block, const CMiniTrans *trans, void *data);
    typedef bool (*MINITRANSVISITFUNC)(CMiniTrans& trans, void *data);
#else
    typedef bool (*MINIBLOCKVISITFUNC)(CMiniBlock& block, void *data);
#endif
    typedef bool (*BLOCKVISITFUNC)(CBlock& block, void *data);

    //-------------------------------------------------------------------------
    extern bool forEveryFullBlockInMemory    (BLOCKVISITFUNC     func, void *data, uint64_t start, uint64_t count, uint64_t skip=1);  // NOLINT
    extern bool forEveryMiniBlockInMemory    (MINIBLOCKVISITFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip=1);  // NOLINT
    extern bool forOnlyMiniBlocks            (MINIBLOCKVISITFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip=1);  // NOLINT
#ifdef MINI_TRANS
    extern bool forOnlyMiniTransactions      (MINITRANSVISITFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip=1);  // NOLINT
#endif
    extern void clearInMemoryCache           (void);

}  // namespace qblocks

//-------------------------------------------------------------------------
#ifdef MINI_TRANS
extern bool visitMiniBlock(CMiniBlock& block, const CMiniTrans *trans, void *data);
#else
extern bool visitMiniBlock(CMiniBlock& block, void *data);
#endif
