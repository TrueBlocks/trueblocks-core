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

//--------------------------------------------------------------------------
class CConsolidator : public CBlockProgress {
  public:
    bool pin;
    blknum_t distFromHead;
    blknum_t prevBlock;
    blknum_t blazeStart;
    blknum_t blazeRipe;
    blknum_t blazeCnt;
    string_q oldStage;
    string_q newStage;
    string_q tmpFile;
    string_q tmp_fn;
    ofstream tmp_stream;
    explicit CConsolidator(const CBlockProgress& prog);
    bool finalize_chunks(void);
    bool consolidate_chunks(void);

  private:
    CConsolidator(void) {
    }
};

//--------------------------------------------------------------------------
// These defines give us control over the scrape when we're testing. A bit cludgy, but effective.
#define CLIENT (isLiveTest() ? (80 + (runs)) : (cons.client))
#define N_BLOCKS (isLiveTest() ? 16 : cons.blazeCnt)
#define MAX_ROWS (isLiveTest() ? 13 : 2000000)
#define SNAP_TO_GRID (isLiveTest() ? 12 : 100000)
#define TEST_RUNS 3

//--------------------------------------------------------------------------
#define LOG_INDEX8(fn, extra)                                                                                          \
    LOG_FN8(fn);                                                                                                       \
    if (isLiveTest()) {                                                                                                \
        LOG8(string_q("Contents of ") + (#fn) + (extra) + ":");                                                        \
        LOG8(substitute(asciiFileToString(fn), "\n", "\n: 8------"));                                                  \
    }
