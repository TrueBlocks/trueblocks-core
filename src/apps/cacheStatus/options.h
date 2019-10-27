#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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

typedef map<string_q,string_q> CIndexHashMap;
//-------------------------------------------------------------------------
class COptions : public COptionsBase {
public:
// BEG_CODE_DECLARE
    bool details;
    bool list;
    blknum_t start;
    blknum_t end;
// END_CODE_DECLARE

    CStatus status;
    string_q mode;
    bool isConfig;
    CIndexHashMap bloomHashes;
    CIndexHashMap indexHashes;

    COptions(void);
    ~COptions(void);

    string_q postProcess(const string_q& which, const string_q& str) const override;
    bool parseArguments(string_q& command) override;
    void Init(void) override;

    bool handle_status(ostream& os);
    bool handle_listing(ostream& os);
    bool handle_config(ostream& os);
    bool handle_config_get(ostream& os);
    bool handle_config_set(ostream& os);
};

//-------------------------------------------------------------------------
extern void loadHashes(CIndexHashMap& map, const string_q& which);
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
    blkrange_t              scanRange;
    CItemCounter(COptions *opt, blknum_t start, blknum_t end) : CCache(), options(opt) {
        cachePtr = NULL;
        indexArray = NULL;
        monitorArray = NULL;
        abiArray = NULL;
        priceArray = NULL;
        ts_array = NULL;
        ts_cnt = 0;
        scanRange.first = start;
        scanRange.second = end;
    }
public:
    CItemCounter(void) : CCache() {}
};
