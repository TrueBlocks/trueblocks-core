#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "etherlib.h"
#include "stake.h"

//#define DEBUGGING 1

class COptions;
//-----------------------------------------------------------------------------
class CCapTable : public map<address_t, CStake> {
  public:
    COptions* pOptions;
    CCapTable() {
    }
};

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
  public:
    blknum_t curBucket = NOPOS;
    string_q totSupply;
    timestamp_t ts;
    string_q report(void);
    uint64_t countNonZero(void);
    CAbi abi;
    uint64_t nRows;
    address_t token;
    bool reverse;
    bool showErrors;
    uint64_t bucketSize;
    CBlock latest;
    CCapTable capTable;
    blknum_t start;
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
