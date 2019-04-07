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

    blknum_t           startScrape;
    blknum_t           scrapeCnt;
    CAccountWatchArray monitors;
    CAccountWatch      primary;
    blknum_t           firstBlockInFile;
    blknum_t           lastBlockInFile;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);
    bool visitBinaryFile(const string_q& path, void *data);
};

extern bool visitIndexFiles  (const string_q& path, void *data);
