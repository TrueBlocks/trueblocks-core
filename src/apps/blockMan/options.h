#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved.
 *------------------------------------------------------------------------*/
#include "etherlib.h"

//-----------------------------------------------------------------------------
#define OP_STATS (1<<0)
#define OP_LIST  (1<<1)
#define OP_CHECK (1<<2)

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    size_t   modes;
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
extern void handle_check(COptions& options);
extern void handle_list(COptions& options);
extern void handle_remove_dups(COptions& options);

//-----------------------------------------------------------------------------
#define F_EMPTY (1<<0)
#define F_FULL (1<<1)

typedef bool (*BLOCKINDEXFUNC)(CBlockIndexItem& bi, void *data);
extern bool forEveryBlockIndexItem(size_t fullMode, BLOCKINDEXFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip=1);  // NOLINT
//extern bool forEveryNonEmptyBlockByNumber(UINT64VISITFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip=1);  // NOLINT
//extern bool forEveryEmptyBlockOnDisc     (BLOCKVISITFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip=1);  // NOLINT
//extern bool forEveryEmptyBlockByNumber   (UINT64VISITFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip=1);  // NOLINT
