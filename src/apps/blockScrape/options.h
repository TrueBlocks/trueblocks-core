#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved.
 *------------------------------------------------------------------------*/
#include "acctlib.h"

//--------------------------------------------------------------------------
class CConsolidator {
public:
    ofstream output;
    ofstream ts_output;
    blknum_t ts_bn;
    blknum_t prevBlock;
    CConsolidator(const string_q& fileName, blknum_t p);

private:
    CConsolidator(void) {}
};

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    timestamp_t latestBlockTs;
    blknum_t    latestBlockNum;

    blknum_t    nBlocks;
    size_t      nBlockProcs;
    size_t      nAddrProcs;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);

    bool handle_scrape(void);
    bool finalize_chunks(CConsolidator *cons);
};

//-----------------------------------------------------------------------------
extern bool copyRipeToStage(const string_q& path, void *data);
