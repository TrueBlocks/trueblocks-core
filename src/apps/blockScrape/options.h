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

enum { TOOL_NONE = 0, TOOL_MONITORS = (1 << 0), TOOL_INDEX = (1 << 1), TOOL_BOTH = (TOOL_MONITORS | TOOL_INDEX) };

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
  public:
    // BEG_CODE_DECLARE
    CStringArray mode;
    CStringArray tool;
    blknum_t n_blocks;
    uint64_t n_block_procs;
    uint64_t n_addr_procs;
    bool pin;
    bool publish;
    // END_CODE_DECLARE

    uint32_t tools = TOOL_NONE;
    timestamp_t latestBlockTs;
    blknum_t latestBlockNum;
    blknum_t maxIndexRows;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);

    bool scrape_once(void);
    bool start_scraper(void);
    bool finalize_chunks(CConsolidator* cons);
};

//-----------------------------------------------------------------------------
extern bool visitCopyRipeToStage(const string_q& path, void* data);
extern bool appendFile(const string_q& toFile, const string_q& fromFile);
extern bool visitMonitor(const string_q& path, void* data);
extern bool isScraperRunning(const string_q& unsearch);
extern bool isAlreadyRunning(const string_q& progName);
