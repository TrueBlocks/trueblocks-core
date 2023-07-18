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

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
  public:
    blknum_t startBlock;
    blknum_t endBlock;

    bool generate = false;
    bool audit = false;
    bool uncles = false;
    period_t by_period = BY_NOTHING;
    bool discrete = false;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);

    bool model_issuance(void);
    bool audit_issuance(void);
    bool show_uncle_blocks(void);
    bool summary_by_period(void);
    bool check_uncles(void);
};

//-----------------------------------------------------------------------------
extern bool visitNonEmptyBlock(CBlock& node, void* data);
extern bool visitEmptyBlock(CBlock& node, void* data);
extern string_q STR_DISPLAY_EXPORT;
extern string_q STR_HEADER_EXPORT;
extern biguint_t operator-(const biguint_t& a, const bigint_t& b);
extern biguint_t operator+(const biguint_t& a, const bigint_t& b);
