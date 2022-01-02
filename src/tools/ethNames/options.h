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

// BEG_ERROR_DEFINES
// END_ERROR_DEFINES

//-----------------------------------------------------------------------------
enum account_t { CUSTOM = (1 << 2), NAMED = (1 << 3), PREFUND = (1 << 4), ALL = (CUSTOM | NAMED) };

//-----------------------------------------------------------------------------
class COptions : public CAbiOptions {
  public:
    // BEG_CODE_DECLARE
    CStringArray terms;
    bool match_case;
    bool prefund;
    bool collections;
    bool tags;
    bool to_custom;
    // END_CODE_DECLARE

    blknum_t latestBlock;

    CAccountNameMap items;
    CStringArray searches;
    string_q searchFields;
    uint64_t types;
    uint64_t nPrefunds{0};

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command) override;
    void Init(void) override;

    void filterNames(void);
    bool addIfUnique(const CAccountName& item);

    void handle_collections(const CStringArray& terms);
    bool handle_clean(void);

    bool finishClean(CAccountName& name);
    bool cleanNames(const string_q& sourceIn, const string_q& destIn);

    // Crud command handling
    CAccountName target;
    bool handle_editcmds(bool autoname);
};
