#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    SFStringArray     requests;
    bool              alone;
    COptionsBlockList blocks;

    COptions(void);
    ~COptions(void);

    bool parseArguments(SFString& command);
    void Init(void);
    SFString postProcess(const SFString& which, const SFString& str) const;
};
