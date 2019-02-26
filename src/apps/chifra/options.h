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
    CAccountWatchArray watches;
    string_q mode;
    blknum_t minWatchBlock;
    blknum_t maxWatchBlock;
    string_q monitorName;
    CArchive txCache;
    string_q remainder;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);
    bool loadMonitors(CToml& toml);

    bool handle_init(void);
    bool handle_freshen(const string_q& pathForce="");
    bool handle_daemon(void);
    bool handle_seed(void);
    bool handle_scrape(void);
    bool handle_config(void);
    bool handle_export(void);
    bool handle_help(void);
    bool handle_names(void);
    bool handle_list(void);
};

//--------------------------------------------------------------------------------
extern string_q colors[];
extern uint64_t nColors;
