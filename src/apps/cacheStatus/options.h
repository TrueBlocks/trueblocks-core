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
#include "indexcache.h"
#include "abicache.h"
#include "slurpcache.h"
#include "namecache.h"
#include "configuration.h"
#include "apiresult.h"

//-------------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    CStatus status;
    string_q mode;
    bool details;
    bool isListing;
    bool isConfig;
    blknum_t start;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);

    void handle_status(ostream& os);
    void handle_listing(ostream& os);
    void handle_config(ostream& os);
    void handle_config_get(ostream& os);
    void handle_config_put(ostream& os);
};

//-------------------------------------------------------------------------
extern bool countFiles(const string_q& path, void *data);
extern bool noteMonitor_light(const string_q& path, void *data);
extern bool noteMonitor(const string_q& path, void *data);
extern bool noteABI(const string_q& path, void *data);
extern bool notePrice(const string_q& path, void *data);
extern bool noteIndex(const string_q& path, void *data);
extern void getIndexMetrics(const string_q& path, uint32_t& nRecords, uint32_t& nAddresses);

//-------------------------------------------------------------------------
class CItemCounter : public CCache {
public:
    COptions               *options;
    CCache                 *cachePtr;
    CIndexCacheItemArray   *indexArray;
    CMonitorCacheItemArray *monitorArray;
    CAbiCacheItemArray     *abiArray;
    CPriceCacheItemArray   *priceArray;
    uint32_t               *ts_array;
    size_t                  ts_cnt;
    blknum_t                skipTo;
    CItemCounter(COptions *opt, blknum_t st=0) : CCache(), options(opt) {
        cachePtr = NULL;
        indexArray = NULL;
        monitorArray = NULL;
        abiArray = NULL;
        priceArray = NULL;
        ts_array = NULL;
        ts_cnt = 0;
        skipTo = st;
    }
public:
    CItemCounter(void) : CCache() {}
};
