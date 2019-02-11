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
    bool isInit;
    bool isImport;
    bool isExport;
    blknum_t minWatchBlock;
    blknum_t maxWatchBlock;
    string_q monitorName;
    CArchive txCache;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);
    bool loadMonitors(CToml& toml);
};

//--------------------------------------------------------------------------------
extern string_q colors[];
extern uint64_t nColors;
