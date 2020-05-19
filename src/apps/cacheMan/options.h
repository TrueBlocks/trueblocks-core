#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
/*
 * Parts of this file were generated with makeClass. Edit only those parts of the code
 * outside of the BEG_CODE/END_CODE sections
 */
#include "acctlib.h"

// BEG_ERROR_DEFINES
// END_ERROR_DEFINES

//-------------------------------------------------------------------------
class COptions : public COptionsBase {
  public:
    // BEG_CODE_DECLARE
    CAddressArray addrs;
    bool list;
    bool no_fix;
    // END_CODE_DECLARE

    CAccountWatchArray monitors;

    COptions(void);
    ~COptions(void);

    bool loadMonitorData(CAppearanceArray& items, const address_t& addr);
    bool parseArguments(string_q& command);
    void Init(void);
};
