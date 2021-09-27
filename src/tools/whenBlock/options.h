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

// BEG_ERROR_DEFINES
#define ERR_OPENINGTIMESTAMPS 1
#define ERR_INVALIDDATE1 2
#define ERR_INVALIDDATE2 3
#define ERR_INVALIDDATE3 4
#define ERR_INVALIDDATE4 5
// END_ERROR_DEFINES

//-----------------------------------------------------------------------------
class CTimeStamper {
  public:
    size_t expected = 0;
    blknum_t prevBn = 0;
    timestamp_t prevTs = blockZeroTs;
    bool verbose = false;
};

//-----------------------------------------------------------------------------
class COptions : public CBlockOptions {
  public:
    // BEG_CODE_DECLARE
    bool list;
    bool timestamps;
    bool check;
    bool fix;
    bool count;
    // END_CODE_DECLARE

    blknum_t latest;
    CTimeStamper checker;
    CNameValueArray requests;
    bool isText;
    blknum_t stop;
    blknum_t cnt;
    CBlockArray corrections;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command) override;
    void Init(void) override;

    void applyFilter(void);
    bool applyCorrections(void);
};

extern bool showSpecials(CNameValue& pair, void* data);
extern bool checkTimestamp(CBlock& block, void* data);
