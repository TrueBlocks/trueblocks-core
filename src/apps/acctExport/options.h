#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "acctlib.h"
#include "tokenlib.h"
#ifndef NOWALLETLIB
#include "walletlib.h"
#endif
#include "options.h"

//-----------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    CAccountWatchArray watches;
    
public:
             COptions        (void);
            ~COptions        (void);

    bool     parseArguments  (string_q& command);
    void     Init            (void);
    bool     loadWatches     (const CToml& toml);
    bool     displayFromCache(void);
};
