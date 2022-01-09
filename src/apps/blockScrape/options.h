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

class CConsolidator;
//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
  public:
    // BEG_CODE_DECLARE
    bool pin;
    bool publish;
    double sleep;
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
};

//-----------------------------------------------------------------------------
extern bool copyRipeToStage(const string_q& path, void* data);
extern bool appendFile(const string_q& toFile, const string_q& fromFile);
extern bool prepareMonitors(const string_q& path, void* data);
extern bool visitToPin(const string_q& chunkId, void* data);
