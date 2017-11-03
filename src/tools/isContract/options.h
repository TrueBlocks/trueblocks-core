#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation. All Rights Reserved.
 *------------------------------------------------------------------------*/
#include "etherlib.h"

#define MAX_ADDRS 1000
//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    SFAddress addrs[MAX_ADDRS];
    uint64_t nAddrs;
    bool diff;
    bool display;

    COptions(void);
    ~COptions(void);

    SFString postProcess(const SFString& which, const SFString& str) const override;
    bool parseArguments(SFString& command) override;
    void Init(void) override;
};
