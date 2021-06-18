#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
/*
 * Parts of this file were generated with makeClass --options. Edit only those parts of
 * the code outside of the BEG_CODE/END_CODE sections
 */
#include "pinlib.h"
#include "status.h"
#include "statusterse.h"
#include "cachebase.h"
#include "chaincache.h"
#include "entitycache.h"
#include "pricecache.h"
#include "monitorcache.h"
#include "indexcache.h"
#include "abicache.h"
#include "slurpcache.h"
#include "namecache.h"
#include "configuration.h"

// BEG_ERROR_DEFINES
// END_ERROR_DEFINES

//-------------------------------------------------------------------------
class COptions : public CAbiOptions {
  public:
    // BEG_CODE_DECLARE
    bool details;
    uint64_t depth;
    bool terse;
    // END_CODE_DECLARE

    CStatus status;
    string_q mode;
    bool isConfig;
    CIndexHashMap bloomHashes;
    CIndexHashMap indexHashes;
    blkrange_t scanRange;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command) override;
    void Init(void) override;

    bool handle_status(ostream& os);
    bool handle_config(ostream& os);
    bool handle_config_get(ostream& os);
};

//-------------------------------------------------------------------------
extern bool countFiles(const string_q& path, void* data);
extern bool countFilesInCache(const string_q& path, void* data);
extern bool listFilesInCache(const string_q& path, void* data);
extern bool noteMonitor_light(const string_q& path, void* data);
extern bool noteMonitor(const string_q& path, void* data);
extern bool noteABI(const string_q& path, void* data);
extern bool notePrice(const string_q& path, void* data);
extern bool noteIndex(const string_q& path, void* data);
extern void getIndexMetrics(const string_q& path, uint32_t& nRecords, uint32_t& nAddresses);

//-------------------------------------------------------------------------
class CItemCounter : public CCache {
  public:
    COptions* options;
    CCache* cachePtr;
    CIndexCacheItemArray* indexArray;
    CMonitorCacheItemArray* monitorArray;
    CAbiCacheItemArray* abiArray;
    CPriceCacheItemArray* priceArray;
    CEntityCacheItemArray* entityArray;
    uint32_t* tsMemMap;
    size_t tsCnt;
    blkrange_t fileRange;
    CItemCounter(COptions* opt) : CCache(), options(opt) {
        cachePtr = NULL;
        indexArray = NULL;
        monitorArray = NULL;
        abiArray = NULL;
        priceArray = NULL;
        tsMemMap = NULL;
        tsCnt = 0;
    }

  public:
    CItemCounter(void) : CCache() {
    }
};
