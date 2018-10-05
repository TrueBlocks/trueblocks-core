#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation. All Rights Reserved.
 *------------------------------------------------------------------------*/
#include "etherlib.h"
//#include "acctlib.h"

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    string_q mode;
    string_q address_list;
    
    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);
};
