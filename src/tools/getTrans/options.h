#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation. All Rights Reserved.
 *------------------------------------------------------------------------*/
#include "etherlib.h"

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    SFString queries;
    bool trace;

    COptions(void);
    ~COptions(void);

    bool parseArguments(SFString& command);
    void Init(void);
    SFString postProcess(const SFString& which, const SFString& str) const;
};

//-----------------------------------------------------------------------------
extern bool visitNonEmptyBlock(CBlock& node, void *data);
extern bool visitEmptyBlock(CBlock& node, void *data);
