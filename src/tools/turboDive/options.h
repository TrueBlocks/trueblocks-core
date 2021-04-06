#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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
 * Parts of this file were generated with makeClass. Edit only those parts of the code
 * outside of the BEG_CODE/END_CODE sections
 */
#include "etherlib.h"
#include "silkworm/db/chaindb.hpp"
#include "classes/tableentry.h"

using namespace silkworm;

// BEG_ERROR_DEFINES
// END_ERROR_DEFINES

//-----------------------------------------------------------------------------
class COptions : public CBlockOptions {
  public:
    // BEG_CODE_DECLARE
    string_q mode;
    string_q datadir;
    string_q name;
    bool goerli;
    bool no_zero;
    // END_CODE_DECLARE

    shared_ptr<lmdb::Environment> lmdb_env{nullptr};
    unique_ptr<lmdb::Transaction> lmdb_txn{nullptr};

    bool first{false};
    CTableEntryArray tables12;
    map<string_q, CTableEntry> tableMap;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command) override;
    void Init(void) override;

    bool loadTables(void);
    bool handle_tables(void);
    bool handle_dump(void);
};
