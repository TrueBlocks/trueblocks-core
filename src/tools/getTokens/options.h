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
#include "tokenbalancerecord.h"

// BEG_ERROR_DEFINES
// END_ERROR_DEFINES

typedef bool (*TOKENVISITFUNC)(CTokenBalanceRecord& token, void* data);
//-----------------------------------------------------------------------------
class COptions : public CHistoryOptions {
  public:
    // BEG_CODE_DECLARE
    CAddressArray addrs;
    bool by_acct;
    bool no_zero;
    // END_CODE_DECLARE

    uint32_t* tsMemMap;
    size_t tsCnt;

    CAddressArray tokens;
    CAddressArray holders;

    CTokenBalanceRecord curToken;
    CMonitor standards;

    tokstate_t modeBits;
    blknum_t latestBlock;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command) override;
    void Init(void) override;
};
