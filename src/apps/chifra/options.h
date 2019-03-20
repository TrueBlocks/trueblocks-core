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
    CAccountWatchArray monitors;
    string_q mode;
    blknum_t minWatchBlock;
    blknum_t maxWatchBlock;
    string_q monitorsPath;
    CArchive txCache;
    string_q flags;
    CAddressArray addrs;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);
    bool createConfigFile(const address_t& addr);

    bool handle_freshen(void);
    bool handle_export (void);
    bool handle_init   (void);
    bool handle_list   (void);
    bool handle_seed   (void);
    bool handle_daemon (void);
    bool handle_scrape (void);
    bool handle_ls     (void);
    bool handle_name   (void);
    bool handle_config (void);
    bool handle_help   (void);
};

//--------------------------------------------------------------------------------
extern bool freshen_internal(const string_q& path, const address_t& addr, const string_q& flags);

//--------------------------------------------------------------------------------
extern string_q colors[];
extern uint64_t nColors;
