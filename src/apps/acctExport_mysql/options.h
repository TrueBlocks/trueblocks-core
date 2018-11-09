#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "acctlib.h"
#include "tokenlib.h"
#ifndef NOWALLETLIB
#include "walletlib.h"
#endif
#include "options.h"
#include "sql_database.h"

//-----------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    bool               sql_on;
    bool               json;
    uint64_t           maxRows;
    CAccountWatchArray watches;
    string_q           transFmt;
    string_q           transExport;

public:
             COptions        (void);
            ~COptions        (void);

    bool     parseArguments  (string_q& command);
    void     Init            (void);
    bool     loadWatches     (const CToml& toml);
//    bool     loadBalances    (void);

    bool     connectDatabase (void);
    bool     displayFromCache(void);
};

//-----------------------------------------------------------------------
extern string_q promoteToFunc(const CTransaction *p);
extern bool     processMigrate(void);
extern bool     processConfigs(const COptions& options);
extern bool     processPrices(void);
extern void     comment(const char *msg);
