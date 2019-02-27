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
    string_q monitorsPath;
//    string_q monitorName;
    CArchive txCache;
    string_q flags;
    string_q address;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);
    bool loadMonitors(CToml& toml);
    bool makeNewMonitor(void);

    bool handle_freshen(void);
    bool handle_export (void);
    bool handle_list   (void);
    bool handle_seed   (void);
    bool handle_daemon (void);
    bool handle_scrape (void);
    bool handle_ls     (void);
    bool handle_config (void);
    bool handle_help   (void);
};

//--------------------------------------------------------------------------------
extern string_q colors[];
extern uint64_t nColors;
