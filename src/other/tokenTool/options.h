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
#include "acctlib.h"
#include "stake.h"

// #define DEBUGGING 1

class COptions;
//-----------------------------------------------------------------------------
class CCapTable : public map<address_t, CStake> {
  public:
    COptions* pOptions;
    CCapTable() {
    }
};

// BEG_ERROR_DEFINES
// END_ERROR_DEFINES

//-----------------------------------------------------------------------------
class COptions : public CAbiOptions {
  public:
    // BEG_CODE_DECLARE
    CAddressArray tokens;
    bool reverse;
    blknum_t start;
    uint64_t bucket;
    uint64_t n_rows;
    bool show_errs;
    // END_CODE_DECLARE

    blknum_t curBucket = NOPOS;
    string_q totSupply;
    timestamp_t ts;
    string_q report(void);
    uint64_t countNonZero(void);
    CCapTable capTable;
    string_q cacheFile;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command) override;
    void Init(void) override;

    void updateHolder(const CTransaction& trans, const address_t& addr, const wei_t& stake);
    wei_t getTokenBalance(const address_t& holder, blknum_t blockNum);
    string_q getTotalSupply(blknum_t blockNum);
};

//-----------------------------------------------------------------------------
extern string_q pad(wei_t in);
extern const string_q fmtOut;
