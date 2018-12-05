#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved.
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "reporter.h"

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    string_q mode;
    bool deep;
    uint32_t skip;
    blknum_t startBlock;
    blknum_t maxBlocks;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);
};

//-----------------------------------------------------------------------------
extern void checkMiniBlocks    (const COptions& opts, CVisitor& reporter);
#ifdef MINI_TRANS
extern bool checkMiniBlock     (CMiniBlock& block, const CMiniTrans *trans, void *data);
#else
extern bool checkMiniBlock     (CMiniBlock& block, void *data);
#endif
extern bool visitNonEmptyBlock (CBlock& node, void *data);
extern bool visitEmptyBlock    (CBlock& node, void *data);
