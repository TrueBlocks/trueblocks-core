#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"

#define MAX_NUMS 100
//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    bool isCheck;
    bool isRaw;
    bool isRange;
    bool terse;
    bool silent;
    SFString txHash;
    SFUint32 nums[MAX_NUMS];
    SFUint32 nNums;
    SFUint32 start;
    SFUint32 stop;

    COptions(void);
    ~COptions(void);

    bool parseArguments(SFString& command);
    void Init(void);
    bool isMulti(void) const;
};

//-----------------------------------------------------------------------------
extern bool     visitNonEmptyBlock(CBlock& node, void *data);
extern bool     visitEmptyBlock(CBlock& node, void *data);
extern uint32_t foundFile(const SFString& path, void *data);
