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
    // END_CODE_DECLARE

    string_q mode;
    CAddressArray addrs;
    string_q freshen_flags;
    string_q tool_flags;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);
    bool createConfigFile(const address_t& addr);

    bool handle_export(void);
    bool handle_commands(void);
};

//--------------------------------------------------------------------------------
extern map<string, string> cmdMap;

//--------------------------------------------------------------------------------
#define RETURN(a)                                                                                                      \
    {                                                                                                                  \
        bool ret = (a);                                                                                                \
        if (isTestMode) {                                                                                              \
            EXIT_NOMSG(0);                                                                                             \
        } else {                                                                                                       \
            EXIT_NOMSG(ret);                                                                                           \
        }                                                                                                              \
    }
