#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
/*
 * This code was generated automatically from grabABI and makeClass. You may
 * edit the file, but keep your edits inside the 'EXISTING_CODE' tags.
 */

//EXISTING_CODE
//EXISTING_CODE

//-----------------------------------------------------------------------
#include "etherlib.h"

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    bool single_on;
    bool accounting_on;
    bool logs_on;
    bool trace_on;
    bool debugger_on;
    bool parse_on;
    bool autocorrect_on;
    blknum_t kBlock;
    blknum_t offset;

    COptions(void);
    ~COptions(void);

    bool parseArguments(SFString& command);
    void Init(void);
};

//EXISTING_CODE
//EXISTING_CODE
