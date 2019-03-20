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
    timestamp_t latestBlockTs;
    uint64_t    maxIndexBytes;
    bool        writeBlocks;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);
    bool isExcluded(const address_t& addr) { return contains(exclusionList, addr); }
};

//-------------------------------------------------------------------------
extern bool handle_freshen(COptions& options);
