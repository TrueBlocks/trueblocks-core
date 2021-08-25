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
#include "collectioncache.h"
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
    CStringArray cachePaths;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command) override;
    void Init(void) override;

    bool handle_status(ostream& os);
    bool handle_config(ostream& os);
    bool handle_config_get(ostream& os);
    bool handle_migrate(void);
    bool handle_migrate_test(void);
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
    CCollectionCacheItemArray* collectionArray;
    blkrange_t fileRange;
    CItemCounter(COptions* opt) : CCache(), options(opt) {
        cachePtr = NULL;
        indexArray = NULL;
        monitorArray = NULL;
        abiArray = NULL;
        priceArray = NULL;
    }

  public:
    CItemCounter(void) : CCache() {
    }
};

//-------------------------------------------------------------------------
class CMigrationChecker {
  public:
    bool needs;
    string_q path;
    string_q type;
    size_t nSeen;
    size_t nMigrated;
    size_t nSkipped;

    CMigrationChecker(const string_q& p, const string_q& t)
        : needs(false), path(p), type(t), nSeen(0), nMigrated(0), nSkipped(0) {
    }

    CMigrationChecker(const CMigrationChecker& mig) {
        nSeen = mig.nSeen;
        nMigrated = mig.nMigrated;
        nSkipped = mig.nSkipped;
        type = mig.type;
        path = mig.path;
    }

    CMigrationChecker& operator+=(const CMigrationChecker& mig) {
        nSeen += mig.nSeen;
        nMigrated += mig.nMigrated;
        nSkipped += mig.nSkipped;
        return *this;
    }

    string_q Report(void) const {
        ostringstream os;
        os << type << ": ";
        os << nSeen << " files seen. ";
        os << nMigrated << " files migrated. ";
        os << (nSeen - nMigrated) << " files up to date. ";
        os << nSkipped << " files skipped.";
        return os.str();
    }

  private:
    CMigrationChecker(void) = delete;
    CMigrationChecker& operator=(const CMigrationChecker&) = delete;
};
