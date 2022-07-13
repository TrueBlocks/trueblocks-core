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

#include "acctlib.h"
#include "bloom.h"

// BEG_ERROR_DEFINES
// END_ERROR_DEFINES

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
  public:
    // BEG_CODE_DECLARE
    // END_CODE_DECLARE

    uint64_t block_cnt;
    uint64_t apps_per_chunk;
    uint64_t unripe_dist;
    uint64_t snap_to_grid;
    uint64_t first_snap;
    bool allow_missing;
    blknum_t start_block{0};

    string_q newStage;
    ofstream tmpStagingStream;
    blknum_t prev_block{0};
    blknum_t nRecsThen{0};
    blknum_t nRecsNow{0};

    COptions(void) {
    }
    ~COptions(void) {
    }
    void Init(void) {
    }
    bool parseArguments(string_q& command) {
        return true;
    }

    bool scrape_blocks(void);
    bool stage_chunks(const string_q& tmpFn);
    bool write_chunks(blknum_t chunkSize, bool snapped);
    bool copyRipeToStage(const string_q& path, bool& snapped);
    bool report(uint64_t nRecsThen, uint64_t nRecsNow) const;
};

//-----------------------------------------------------------------------------
extern bool writeIndexAsBinary(const string_q& outFn, const CStringArray& lines);
extern bool freshenTimestampsAppend(blknum_t firstBlock, blknum_t nBlocks);
