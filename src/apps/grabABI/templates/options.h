#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation. All Rights Reserved.
 *------------------------------------------------------------------------*/
#include "etherlib.h"

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    SFString mode;
    bool single_on;
    bool accounting_on;
    bool logs_on;
    bool trace_on;
    bool bloom_on;
    bool debugger_on;
    bool parse_on;
    blknum_t kBlock;

    COptions(void);
    ~COptions(void);

    bool parseArguments(SFString& command);
    void Init(void);
};

//-----------------------------------------------------------------------------
extern bool visitNonEmptyBlock(CBlock& node, void *data);
extern bool visitEmptyBlock(CBlock& node, void *data);
