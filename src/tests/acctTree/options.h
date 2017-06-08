#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation. All Rights Reserved.
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "acctlib.h"

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    bool option1;
    bool option2;

    COptions(void);
    ~COptions(void);

    bool parseArguments(SFString& command);
    void Init(void);
};

//-----------------------------------------------------------------------------
extern bool visitNonEmptyBlock(CBlock& node, void *data);
extern bool visitEmptyBlock(CBlock& node, void *data);
