#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved.
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "scraper_context.h"

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    blknum_t    startBlock;
    blknum_t    endBlock;
    blknum_t    maxBlocks;
    string_q    exclusionList;
    bool        writeBlocks;
    uint64_t    bitBound;
    timestamp_t latestBlockTs;
    bool        addrIndex;
    bool        consolidate;
    uint64_t    maxIdxSize;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);
    bool updateIndex(blknum_t bn);
    bool isExcluded(const address_t& addr) { return contains(exclusionList, addr); }
};

//-------------------------------------------------------------------------
extern bool establishBlockIndex(void);
extern bool handle_freshen(COptions& options);
