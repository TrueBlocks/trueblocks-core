#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

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
        SFGas    gasUsed;
        SFGas    gasPrice;
        SFGas    gasAllowed;
        bool     isError;
        char     from [41];
        char     to   [41];
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

        CSharedResource blocksOnDisc;
        CSharedResource transOnDisc;

        SFString        blockFile;
        SFString        transFile;

        CMiniBlock     *blocks;
        CMiniTrans     *trans;

    private:
        blknum_t        nBlocks;
        txnum_t         nTrans;
        uint64_t        start;
        uint64_t        count;
    };

}  // namespace qblocks

