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
#include "pinlib.h"
#include "acctlib.h"

// BEG_ERROR_DEFINES
// END_ERROR_DEFINES

//--------------------------------------------------------------------------
class CConsolidator {
  public:
    ofstream tmp_file;
    string_q tmp_fn;

    blknum_t prevBlock;
    explicit CConsolidator(blknum_t p);

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
    bool pin;
    bool publish;
    // END_CODE_DECLARE

    timestamp_t latestBlockTs;
    blknum_t latestBlockNum;
    // FIX_THIS_CODE
    blknum_t maxIndexRows;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);

    bool handle_scrape(void);
    bool finalize_chunks(CConsolidator* cons);

    bool hasPinataKeys(void);
};

//-----------------------------------------------------------------------------
extern bool visitCopyRipeToStage(const string_q& path, void* data);
extern string_q handle_reporting(void);
extern bool appendFile(const string_q& toFile, const string_q& fromFile);
