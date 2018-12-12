#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved.
 *------------------------------------------------------------------------*/
#include "etherlib.h"

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    blknum_t  start;
    blknum_t  end;
    blknum_t  maxBlocks;
    bool      writeBlocks;
    string_q  exclusions;
    bool      keepAddrIdx;
    uint64_t  bitBound;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);
    bool updateIndex(blknum_t bn);
    bool isExcluded(const address_t& addr) { return contains(exclusions, addr); }
};

//-------------------------------------------------------------------------
class CScraperCtx {
public:
    COptions        *opts;
    CBlock          *pBlock;
    CTransaction    *pTrans;
    CBloomArray     blooms;
    uint64_t         traceCount;
    uint64_t         maxTraceDepth;
    bool             reported;
    uint64_t         nAccounts;
    uint64_t         totAccounts;
    string_q         potList; // potential addresses from 'input' or log 'data'

    CScraperCtx(COptions *o) :
        opts(o),
        pBlock(NULL), pTrans(NULL),
        traceCount(0), maxTraceDepth(0), reported(false), nAccounts(0), totAccounts(0)
        { blooms.resize(1); blooms.at(0) = 0; }
    void addToBloom(const address_t& addr);
    bool scrape(CBlock& block);
};

//-----------------------------------------------------------------------------
extern bool visitNonEmptyBlock(CBlock& node, void *data);
extern bool visitEmptyBlock(CBlock& node, void *data);

//-------------------------------------------------------------------------
extern bool establishBlockIndex(void);
extern bool freshenLocalCache(COptions& options);
