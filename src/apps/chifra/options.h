#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved.
 *------------------------------------------------------------------------*/
#include "acctlib.h"

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
public:
// BEG_CODE_DECLARE
    uint32_t sleep;
// END_CODE_DECLARE

    string_q   mode;
    useconds_t scrapeSleep;

    string_q freshen_flags;
    string_q tool_flags;
    CAddressArray addrs;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);
    bool createConfigFile(const address_t& addr);

    bool handle_list     (void);
    bool handle_export   (void);
    bool handle_leech    (void);
    bool handle_scrape   (void);
    bool handle_slurp    (void);
    bool handle_quotes   (void);
    bool handle_status   (void);
    bool handle_rm       (void);
    bool handle_data     (void);
    bool handle_config   (void);
};

//--------------------------------------------------------------------------------
extern bool freshen_internal(freshen_e mode, const CAddressArray& list, const string_q& tool_flags, const string_q& freshen_flags);

//--------------------------------------------------------------------------------
extern string_q colors[];
extern uint64_t nColors;
#define indexFolder_sorted    (getIndexPath("sorted/"))
