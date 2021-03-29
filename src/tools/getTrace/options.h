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

// BEG_ERROR_DEFINES
// END_ERROR_DEFINES

//-----------------------------------------------------------------------------
class COptions : public CAbiOptions {
  public:
    // BEG_CODE_DECLARE
    bool articulate;
    bool statediff;
    bool count;
    bool skip_ddos;
    uint64_t max_traces;
    string_q filter;
    // END_CODE_DECLARE

    CTraceFilterArray filters;
    COptionsTransList transList;
    bool first;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command) override;
    void Init(void) override;

    bool extractBlocksFromFilter(blknum_t& b1, const string_q& p1, blknum_t& b2, const string_q& p2);
};

//-----------------------------------------------------------------------------
extern bool visitTransaction(CTransaction& trans, void* data);
extern bool displayAsCreation(COptions* opt, const CTrace& trace);
extern bool displayAsSuicide(COptions* opt, const CTrace& trace);
extern bool displayAsTrace(COptions* opt, const CTrace& trace);
