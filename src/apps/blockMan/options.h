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
    bool deepCheck;
    bool noEmpty;
    bool emptyOnly;
    int skip;
    string_q mode;
    FILE *fp;
    uint64_t lastBlock;
    uint64_t startBlock;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);
};

//-----------------------------------------------------------------------------
extern bool visitNonEmptyBlock(CBlock& node, void *data);
extern bool visitEmptyBlock(CBlock& node, void *data);
