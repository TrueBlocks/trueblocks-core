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
// END_ERROR_DEFINES

//-------------------------------------------------------------------------
class COptions : public CAbiOptions {
  public:
    // BEG_CODE_DECLARE
    bool check;
    bool extract;
    bool stats;
    bool blooms;
    // END_CODE_DECLARE

    CPinnedChunkArray pins;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command) override;
    void Init(void) override;

    bool handle_list(void);
    bool handle_check(void);
    bool handle_extract(void);
};
