#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "acctlib.h"

//-------------------------------------------------------------------------
class CStats {
public:
    uint64_t nDups;
    uint64_t nReversals;
    uint64_t nFixed;
    uint64_t nRecords;
    uint64_t nTruncs;
    CStats(void) : nDups(0), nReversals(0), nFixed(0), nRecords(0), nTruncs(0)
        {}
};

//-------------------------------------------------------------------------
class COptions;
typedef bool (*CACHEFILTERFUNC)(CAcctCacheItemArray& dataArray, const CAcctCacheItem& item);

//-------------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    CStats stats;
    CStringArray filenames;
    string_q mode;
    blknum_t trunc;
    blknum_t maxBlock;
    bool asData;
    uint64_t skip;
    bool isImport;
    bool isRemove;
    CAcctCacheItemArray removals;
    CAccountWatchArray watches;

    COptions(void);
    ~COptions(void);

    bool handleMerge    (void) const;
    bool handleSort     (void) const;
    bool handleImport   (void) const;
    bool handleRemove   (void) const;
    bool handleRead     (const string_q& mode, size_t filesToUse, CAcctCacheItemArray& dataArray) const;
    bool handleWrite    (const string_q& outputFilename, const CAcctCacheItemArray& dataArray, CACHEFILTERFUNC filterFunc) const;
    bool loadWatches     (const CToml& toml);

    bool parseArguments(string_q& command);
    void Init(void);
};

//-------------------------------------------------------------------------
extern bool handleList      (COptions& options);
extern bool handleFix       (COptions& options);
extern bool handleCacheBals (COptions& options);
extern bool listBalances    (COptions& options);
extern int  sortByBlock     (const void *v1, const void *v2);
extern void writeLastBlock  (blknum_t bn);

//-------------------------------------------------------------------------
extern const char *STR_DEFAULT_DISPLAY;
extern const char *STR_DATA_DISPLAY;
