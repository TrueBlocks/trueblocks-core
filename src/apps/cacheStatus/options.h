#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
/*
 * Parts of this file were generated with makeClass --options. Edit only those parts of
 * the code outside of the BEG_CODE/END_CODE sections
 */
#include "acctlib.h"
#include "cachebase.h"
#include "cache.h"
#include "chaincache.h"
#include "abicacheitem.h"
#include "abicache.h"
#include "monitorcacheitem.h"
#include "monitorcache.h"
#include "indexcacheitem.h"
#include "indexcache.h"
#include "namecache.h"
#include "slurpcache.h"
#include "status.h"
#include "statusterse.h"

// BEG_ERROR_DEFINES
// END_ERROR_DEFINES

//-------------------------------------------------------------------------
class COptions : public CAbiOptions {
  public:
    // BEG_CODE_DECLARE
    string_q mode;
    // END_CODE_DECLARE

    uint64_t depth{1};
    CStatus status;
    string_q origMode;
    CIndexHashMap bloomHashes;
    CIndexHashMap indexHashes;
    blkrange_t scanRange;
    CMetaData meta;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command) override;
    void Init(void) override;

    bool handle_status(ostream& os);
    bool handle_migrate(const CStringArray& cachePaths);
    bool handle_migrate_test(const CStringArray& cachePaths);
};

//-------------------------------------------------------------------------
extern bool noteMonitor_light(const string_q& path, void* data);
extern bool noteMonitor(const string_q& path, void* data);
extern bool noteABI(const string_q& path, void* data);
extern bool noteIndex(const string_q& path, void* data);

//-------------------------------------------------------------------------
class CItemCounter : public CCache {
  public:
    COptions* options;
    CCache* cachePtr;
    CIndexCacheItemArray* indexArray;
    CMonitorCacheItemArray* monitorArray;
    CAbiCacheItemArray* abiArray;
    blkrange_t fileRange;
    CItemCounter(COptions* opt) : CCache(), options(opt) {
        cachePtr = NULL;
        indexArray = NULL;
        monitorArray = NULL;
        abiArray = NULL;
    }

  public:
    CItemCounter(void) : CCache() {
    }
};

extern bool countFiles(const string_q& path, void* data);
extern bool countFilesInCache(const string_q& path, void* data);
