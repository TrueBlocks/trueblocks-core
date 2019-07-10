#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved.
 *------------------------------------------------------------------------*/
#include "acctlib.h"
#include "scraper_context.h"

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    timestamp_t latestBlockTs;
    blknum_t    latestBlockNum;
    uint64_t    maxIndexRows;
#ifdef OLD_CODE
    blknum_t    maxBlocks;
    bool        writeBlocks;
    blknum_t    startBlock;
    blknum_t    endBlock;
#else
    blknum_t    nBlocks;
    size_t      nBlockChans;
    size_t      nAddrChans;
#endif

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);
};

//-------------------------------------------------------------------------
extern bool handle_scrape(COptions &options);
#define indexFolder_sorted    (getCachePath("addr_index/sorted/"))
