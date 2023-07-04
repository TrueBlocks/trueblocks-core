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
#include "traversers.h"

// BEG_ERROR_DEFINES
// END_ERROR_DEFINES

//-----------------------------------------------------------------------
class COptions : public CStatementOptions {
  public:
    // BEG_CODE_DECLARE
    // END_CODE_DECLARE

    bool skip_ddos{true};
    uint64_t max_traces{250};
    CMonitorArray allMonitors;
    const CMonitor* curMonitor;

    CBlockAddressMap prefundAddrMap;
    CBlockAddressMap blkRewardMap;

    // abiMap allows fast access to abis
    CAddressUintMap abiMap;

    blkrange_t fileRange;
    blkrange_t exportRange;

    string_q className;
    CMetaData meta;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);

    bool loadMonitors(void);

    bool handle_traversers(void);

    bool articulateAll(CTransaction& trans);
};

//--------------------------------------------------------------------------------
extern bool visitOnLoad(CAppearance_mon& app, void* data);
