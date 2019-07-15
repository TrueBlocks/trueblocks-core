#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved.
 *------------------------------------------------------------------------*/
#include "acctlib.h"

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    timestamp_t latestBlockTs;
    blknum_t    latestBlockNum;

    blknum_t    nBlocks;
    size_t      nBlockProcs;
    size_t      nAddrProcs;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);
};
