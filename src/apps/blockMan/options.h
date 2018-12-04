#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved.
 *------------------------------------------------------------------------*/
#include "etherlib.h"

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    uint64_t modes;
    blknum_t startBlock;
    blknum_t nBlocks;
    blknum_t skip;
    bool     incEmpty;
    bool     incFull;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);
};

//----------------------------------------------------------------------------------
class CStatistics {
public:
    COptions *opt;
    uint64_t fullCnt;
    uint64_t emptyCnt;
    uint64_t visited;
    CStatistics(COptions *o) : opt(o), fullCnt(0), emptyCnt(0), visited(0) { }
};

//-----------------------------------------------------------------------------
#define OP_STATS   (1<<1)
#define OP_LIST    (1<<2)
#define OP_CHECK   (1<<3)

//-----------------------------------------------------------------------------
extern void handle_check(COptions& options);
extern void handle_list(COptions& options);
extern void handle_remove_dups(COptions& options);
