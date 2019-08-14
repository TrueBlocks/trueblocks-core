#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "acctlib.h"

#include "status.h"
#include "chaincache.h"
#include "pricecache.h"
#include "monitorcache.h"
#include "addresscache.h"
#include "abicache.h"
#include "slurpcache.h"

//-------------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    CStatus status;
    string_q mode;
    bool details;
    bool ls;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);

    void doStatus(ostream& os);
    void doListing(ostream& os);
};

//-------------------------------------------------------------------------
extern bool countFiles(const string_q& path, void *data);
extern bool noteMonitor(const string_q& path, void *data);
extern bool noteABI(const string_q& path, void *data);
extern bool notePrice(const string_q& path, void *data);

//-------------------------------------------------------------------------
class CItemCounter : public CCache {
public:
    COptions *options;
    CCache *cachePtr;
    CMonitorCacheItemArray *monitorArray;
    CAbiCacheItemArray *abiArray;
    CPriceCacheItemArray *priceArray;
    CItemCounter(COptions *opt) : CCache(), options(opt) { cachePtr = NULL; monitorArray = NULL; abiArray = NULL; priceArray = NULL; }
public:
    CItemCounter(void) : CCache() {}
};
