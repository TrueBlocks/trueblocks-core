#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved.
 *------------------------------------------------------------------------*/
/*
 * Parts of this file were generated with makeClass. Edit only those parts of the code
 * outside of the BEG_CODE/END_CODE sections
 */
#include "acctlib.h"

// BEG_ERROR_DEFINES
// END_ERROR_DEFINES

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
    explicit CConsolidator(blknum_t p);
    // TS_SCRAPER
    //    bool checkTimestamps(void);

  private:
    CConsolidator(void) {
    }
};

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
  public:
    // BEG_CODE_DECLARE
    blknum_t n_blocks;
    uint64_t n_block_procs;
    uint64_t n_addr_procs;
    // END_CODE_DECLARE

    timestamp_t latestBlockTs;
    blknum_t latestBlockNum;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);

    bool handle_scrape(void);
    bool finalize_chunks(CConsolidator* cons);
};

//-----------------------------------------------------------------------------
extern bool copyRipeToStage(const string_q& path, void* data);
