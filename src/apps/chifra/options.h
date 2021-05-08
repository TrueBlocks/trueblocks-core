#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved.
 *------------------------------------------------------------------------*/
#include "acctlib.h"

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
  public:
    // BEG_CODE_DECLARE
    // END_CODE_DECLARE

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command) {
        return true;
    }
    void Init(void);

    bool call_command(int argc, const char* argv[]);
};

//-----------------------------------------------------------------------------
extern map<string, string> chifraCmdMap;
extern const char* STR_CHIFRA_HELP;
