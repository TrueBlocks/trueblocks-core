#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "acctlib.h"

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
public:

    blknum_t           earliestStart;
    blknum_t           scrapeCnt;
    CAccountWatchArray monitors;
    CAccountWatch      primary;
    blknum_t           lastBlockInFile;
    size_t             visitTypes;
    bool               useBlooms;
    ofstream           log_file;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);
    bool visitBinaryFile(const string_q& path, void *data);
    void moveToProduction(void);
};

#define VIS_FINAL   (1<<1)
#define VIS_STAGING (1<<2)
#define VIS_PENDING (1<<3)

extern bool visitFinalIndexFiles(const string_q& path, void *data);
extern bool visitStagingIndexFiles(const string_q& path, void *data);
extern bool visitPendingIndexFiles(const string_q& path, void *data);

//#define LOG(a) { a; }
#define LOG(a) { }
