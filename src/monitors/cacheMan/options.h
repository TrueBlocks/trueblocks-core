#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"

class COptions : public COptionsBase {
public:
    SFStringArray filenames;
    SFString mode;
    blknum_t trunc;
    uint64_t extractID;
    bool isMerge;
    bool blockOnly;
    bool wantsStats;
    SFIntArray renums;
    SFUint32 skip;

    COptions(void);
    ~COptions(void);

    bool parseArguments(SFString& command);
    void Init(void);
};

//-----------------------------------------------------------------------------
extern bool visitNonEmptyBlock(CBlock& node, void *data);
extern bool visitEmptyBlock(CBlock& node, void *data);
