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
#include "etherlib.h"

// BEG_ERROR_DEFINES
#define ERR_INVALIDSKIPVAL 1
// END_ERROR_DEFINES

//-----------------------------------------------------------------------------
class COptions : public CBlockOptions {
  public:
    // BEG_CODE_DECLARE
    bool list;
    bool timestamps;
    uint64_t skip;
    // END_CODE_DECLARE

    CNameValueArray requests;
    bool isText;
    blknum_t stop;
    blknum_t cnt;
    bool hasHelp;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command) override;
    void Init(void) override;

    void applyFilter(void);
};

extern bool showSpecials(CNameValue& pair, void* data);
