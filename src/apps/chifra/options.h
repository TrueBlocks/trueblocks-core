#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved.
 *------------------------------------------------------------------------*/
#include "acctlib.h"

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    bool       stats;
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
    bool handle_balances (void);
    bool handle_leech    (void);
    bool handle_scrape   (void);
    bool handle_slurp    (void);
    bool handle_quotes   (void);
    bool handle_ls       (void);
    bool handle_rm       (void);
    bool handle_data     (void);
    bool handle_config   (void);
};

//--------------------------------------------------------------------------------
extern bool freshen_internal(freshen_t mode, const CAddressArray& list, const string_q& tool_flags, const string_q& freshen_flags);

//--------------------------------------------------------------------------------
extern string_q colors[];
extern uint64_t nColors;
#define indexFolder_sorted    (getCachePath("addr_index/sorted/"))
