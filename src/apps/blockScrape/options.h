#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved.
 *------------------------------------------------------------------------*/
/*
 * Parts of this file were generated with makeClass --options. Edit only those parts of
 * the code outside of the BEG_CODE/END_CODE sections
 */
#include "pinlib.h"
#include "acctlib.h"

// BEG_ERROR_DEFINES
// END_ERROR_DEFINES

enum { TOOL_NONE = 0, TOOL_MONITORS = (1 << 0), TOOL_INDEX = (1 << 1), TOOL_BOTH = (TOOL_MONITORS | TOOL_INDEX) };
typedef enum { STATE_STOPPED, STATE_RUNNING, STATE_PAUSED } ScrapeState;

class CConsolidator;
//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
  public:
    // BEG_CODE_DECLARE
    string_q mode;
    CStringArray tool;
    uint64_t n_blocks;
    uint64_t n_block_procs;
    uint64_t n_addr_procs;
    bool pin;
    double sleep;
    bool staging;
    bool unripe;
    bool cache_txs;
    bool cache_traces;
    string_q load;
    // END_CODE_DECLARE

    ScrapeState state;
    string_q controlFile;
    string_q stateStr;
    uint32_t tools = TOOL_NONE;
    timestamp_t latestBlockTs;
    blknum_t latestBlockNum;
    CPinnedChunkArray pinList;
    CApiKey lic;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);

    bool start_scraper(void);
    bool scrape_blocks(void);
    bool scrape_monitors(void);

    bool changeState(void);
    ScrapeState getCurrentState(string_q& current);
    void cleanupAndQuit(void) {
        mode = "quit";
        changeState();
    }
};

//-----------------------------------------------------------------------------
extern bool copyRipeToStage(const string_q& path, void* data);
extern bool appendFile(const string_q& toFile, const string_q& fromFile);
extern bool prepareMonitors(const string_q& path, void* data);
extern bool visitToPin(const string_q& chunkId, void* data);
