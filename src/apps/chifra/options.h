#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved.
 *------------------------------------------------------------------------*/
/*
 * Parts of this file were generated with makeClass. Edit only those parts of the code
 * outside of the BEG_CODE/END_CODE sections
 */
#include "acctlib.h"

// BEG_ERROR_DEFINES
// END_ERROR_DEFINES

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
  public:
    // BEG_CODE_DECLARE
    uint32_t sleep;
    // END_CODE_DECLARE

    string_q mode;
    useconds_t scrapeSleep;

    string_q freshen_flags;
    string_q tool_flags;
    CAddressArray addrs;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);
    bool createConfigFile(const address_t& addr);

    bool handle_export(void);
    bool handle_scrape(void);
    bool handle_rm(void);
    bool handle_data(void);
};

extern bool freshen_internal(freshen_e mode, CMonitorArray& list, const string_q& tool_flags,
                             const string_q& freshen_flags);

//--------------------------------------------------------------------------------
extern string_q colors[];
extern uint64_t nColors;
#define indexFolder_sorted (getIndexPath("sorted/"))
#define LOG_CALL(a)                                                                                                    \
    { LOG4(bWhite, l_funcName, " ----> ", (isTestMode() ? substitute((a), getCachePath(""), "$CACHE/") : (a)), cOff); }
