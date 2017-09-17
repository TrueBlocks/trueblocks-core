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
    CAccountNameArray accounts;
    CFilename namesFile;
    SFString addr;
    SFString name;
    SFString source;
    bool all;
    bool count;
    bool matchCase;
    bool open;
    bool list;
    bool addrOnly;
    bool isEdit;

    COptions(void);
    ~COptions(void) {}

    bool parseArguments(SFString& command);
    void Init(void);
    bool loadData(void);
    SFString showName(void);
};
