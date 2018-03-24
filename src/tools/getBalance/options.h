#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation. All Rights Reserved.
 *------------------------------------------------------------------------*/
#include "etherlib.h"

//-----------------------------------------------------------------------------
class CState {
public:
    bool needsNewline;
    blknum_t latestBlock;
    SFUintBN totalVal;
    SFUintBN lastBal;
    SFAddress curAddr;
    CState(void) { needsNewline = true; latestBlock = getLatestBlockFromClient(); totalVal = 0; lastBal = 0; }
};

//-----------------------------------------------------------------------------
class COptions : public CBlockOptions {
public:
    CState state;
    SFString addrs;
    bool asData;
    bool noZero;
    bool total;
    bool changes;

    COptions(void);
    ~COptions(void);

    bool parseArguments(SFString& command) override;
    void Init(void) override;
    SFString postProcess(const SFString& which, const SFString& str) const override;
};
