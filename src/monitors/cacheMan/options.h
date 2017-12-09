#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "acctcacheitem.h"

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
class COptions : public COptionsBase {
public:
    CStats stats;
    SFStringArray filenames;
    SFString mode;
    blknum_t trunc;
    uint64_t extractID;
    bool isMerge;
    bool isSplit;
    bool isExtract;
    bool asData;
    bool wantsStats;
    uint64_t skip;

    COptions(void);
    ~COptions(void);

    bool parseArguments(SFString& command);
    void Init(void);
};

//-------------------------------------------------------------------------
extern bool processList(COptions& options);
extern bool processFix(COptions& options);
extern bool processMerge(COptions& options);
extern bool processExtract(COptions& options);
extern bool processSplit(COptions& options);
extern int  sortByBlock(const void *v1, const void *v2);
extern int  sortByAccount(const void *v1, const void *v2);

//-------------------------------------------------------------------------
extern const char *STR_DEFAULT_DISPLAY;
extern const char *STR_TERSE_DISPLAY;
