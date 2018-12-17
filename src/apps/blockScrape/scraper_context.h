#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved.
 *------------------------------------------------------------------------*/
#include "etherlib.h"

class COptions;
//-------------------------------------------------------------------------
class CScraperContext {
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
    bool             blockOkay;
    bool             bloomOkay;

    CScraperContext(COptions *o, CBlock *b);

    void addToBloom(const address_t& addr);
    bool scrape(CBlock& block);
};
