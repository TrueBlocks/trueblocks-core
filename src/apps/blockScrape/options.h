#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
/*
 * Parts of this file were generated with makeClass --options. Edit only those parts of
 * the code outside of the BEG_CODE/END_CODE sections
 */
#include "pinlib.h"
#include "acctlib.h"

// BEG_ERROR_DEFINES
// END_ERROR_DEFINES

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
  public:
    // BEG_CODE_DECLARE
    bool pin;
    bool publish;
    uint64_t block_cnt;
    uint64_t block_chan_cnt;
    uint64_t addr_chan_cnt;
    uint64_t apps_per_chunk;
    uint64_t unripe_dist;
    uint64_t snap_to_grid;
    uint64_t first_snap;
    bool allow_missing;
    uint64_t n_test_runs;
    // END_CODE_DECLARE

    string_q tmpFile;
    string_q oldStage;
    string_q newStage;
    ofstream tmp_stream;
    string_q tmp_fn;
    blknum_t prev_block{0};
    blknum_t blaze_ripe{0};
    blknum_t blaze_start{0};
    timestamp_t latestBlockTs;
    blknum_t latestBlockNum;
    CPinnedChunkArray pinList;
    CApiKey lic;
    blknum_t distFromHead;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);

    bool start_scraper(void);
    bool scrape_blocks(void);
    bool stage_chunks(void);
    bool write_chunks(blknum_t chunkSize, bool atLeastOnce);
    bool isSnapToGrid(blknum_t bn) {
        return bn > first_snap && !(bn % snap_to_grid);
    }
};

//-----------------------------------------------------------------------------
extern bool copyRipeToStage(const string_q& path, void* data);
extern bool prepareMonitors(const string_q& path, void* data);
extern bool visitToPin(const string_q& chunkId, void* data);
bool writeIndexAsBinary(const string_q& outFn, const CStringArray& lines, CONSTAPPLYFUNC pinFunc, void* pinFuncData);
