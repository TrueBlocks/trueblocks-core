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
    blknum_t  startBlock;
    blknum_t  endBlock;
    blknum_t  maxBlocks;
    string_q  exclusionList;
    bool      writeBlocks;
    bool      keepAddrIdx;
    uint64_t  bitBound;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);
    bool updateIndex(blknum_t bn);
    bool isExcluded(const address_t& addr) { return contains(exclusionList, addr); }
};

//-------------------------------------------------------------------------
class CScraperCtx {
public:
    COptions        *opts;
    CBlock          *pBlock;
    CTransaction    *pTrans;
    CBloomArray      bloomList;
//    CAddressArray    addrList;
    string_q         potList;
    uint64_t         traceCount;
    uint64_t         maxTraceDepth;
    bool             reported;
    uint64_t         nAddrsInBloom;
    uint64_t         nAddrsInBlock;

    CScraperCtx(COptions *o);
    void addToBloom(const address_t& addr);
    bool scrape(CBlock& block);
};

//-------------------------------------------------------------------------
extern bool establishBlockIndex(void);
extern bool handle_freshen(COptions& options);
