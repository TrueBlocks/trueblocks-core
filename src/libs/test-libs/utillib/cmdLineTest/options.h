#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation. All Rights Reserved.
 *------------------------------------------------------------------------*/
#include "etherlib.h"

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    bool boolOption;
    bool boolSet;
    int64_t numOption;
    SFString stringOption;
    int32_t testNum;

    COptions(void);
    ~COptions(void);

    bool parseArguments(SFString& command);
    void Init(void);
    void output(void) const {
        cout << "boolOption: "   << boolOption << "\n";
        cout << "boolSet: "      << boolSet << "\n";
        cout << "numOption: "    << numOption << "\n";
        cout << "stringOption: " << stringOption << "\n";
        cout << "testNum: "      << testNum << "\n";
    }
};
