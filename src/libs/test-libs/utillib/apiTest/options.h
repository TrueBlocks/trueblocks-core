#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation. All Rights Reserved.
 *------------------------------------------------------------------------*/
#include "etherlib.h"

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    COptions(void);
    ~COptions(void);

    bool parseArguments(SFString& command);
    void Init(void);
};
