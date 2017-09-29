#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation. All Rights Reserved.
 *------------------------------------------------------------------------*/
#include "etherlib.h"

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    SFString tokens;
    SFString holders;
    SFString blocks;
    bool asData;
    bool byAccount;
    bool noZero;

    COptions(void);
    ~COptions(void);

    bool parseArguments(SFString& command) override;
    void Init(void) override;
    SFString postProcess(const SFString& which, const SFString& str) const override;
};
