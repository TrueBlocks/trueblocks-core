#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved.
 *------------------------------------------------------------------------*/
#include "etherlib.h"

class COptions;
//-------------------------------------------------------------------------
class CScraper {
public:
    COptions     *options;
    string_q      status;

    CUniqueState  addrList;
    CBlock        block;

    CTransaction *pTrans;
    string_q      potList;
    uint64_t      traceCount;
    uint64_t      curSize;
    uint64_t      maxTraceDepth;
    uint64_t      nAddrsInBlock;

    CScraper(COptions *o, blknum_t num);

    bool scrapeBlock(void);
    bool scrapeTransaction(CTransaction *pT);
    string_q report(uint64_t last);

    void noteAddress(const address_t& addr, bool isMiner=false);
    void updateAddrIndex(void);
};

extern bool notePotential(const CAddressAppearance& item, void *data);
extern void foundPotential(ADDRESSFUNC func, void *data, blknum_t bn, blknum_t tx, blknum_t tc, const string_q& potList);
