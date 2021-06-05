#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved.
 *------------------------------------------------------------------------*/
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
    CPinnedChunkArray pinList;

    explicit CConsolidator(void);
    bool stage_chunks(void);
    bool consolidate_chunks(void);
    bool write_chunks(blknum_t chunkSize, bool once);
    void Format(ostream& os) const;
    string_q Format(void) const {
        ostringstream os;
        Format(os);
        return os.str();
    }
    friend ostream& operator<<(ostream& os, const CConsolidator& it);
};

//--------------------------------------------------------------------------
// These defines give us control over blockScrape while testing. This is  VERY
// hacky. Snap-to-grid allows for corrections to the data without repbulishing
// the entire index (we can the broken files between snaps). We don't start until
// the Sept. 2016 dDos attacks and the continue to end of chain
#define MAX_ROWS blknum_t(isLiveTest() ? 6000 : 2000000)
#define SNAP_TO_GRID_BLKS blknum_t(isLiveTest() ? 30 : 100000)
#define FIRST_SNAP_TO_GRID blknum_t(isLiveTest() ? 0 : 2250000)
#define TEST_RUNS blknum_t(isLiveTest() ? 10 : NOPOS)

//-------------------------------------------------------------------------
inline ostream& operator<<(ostream& os, const CConsolidator& it) {
    it.Format(os);
    os << endl;
    return os;
}
