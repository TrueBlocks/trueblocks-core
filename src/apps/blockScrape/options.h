#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved.
 *------------------------------------------------------------------------*/
#include "acctlib.h"

//--------------------------------------------------------------------------
class CConsolidator {
public:
    ofstream tmp_file;
    string_q tmp_fn;

// TS_SCRAPER
//    string_q ts_fn;
//    ofstream ts_file;
//    blknum_t ts_bn;

    blknum_t prevBlock;
    CConsolidator(blknum_t p);
// TS_SCRAPER
//    bool checkTimestamps(void);

private:
    CConsolidator(void) {}
};

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
public:
// BEG_CODE_DECLARE
// END_CODE_DECLARE

    timestamp_t latestBlockTs;
    blknum_t    latestBlockNum;

    blknum_t    n_blocks;
    size_t      n_block_procs;
    size_t      n_addr_procs;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);

    bool handle_scrape(void);
    bool finalize_chunks(CConsolidator *cons);
};

//-----------------------------------------------------------------------------
extern bool copyRipeToStage(const string_q& path, void *data);
