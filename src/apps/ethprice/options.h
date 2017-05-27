#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

//--------------------------------------------------------------
class COptions : public COptionsBase {
public:
    bool freshen;
    SFUint32 dispLevel;
    SFUint32 freq;
    SFUint32 hour;

    COptions(void);
    ~COptions(void) {}

    bool parseArguments(SFString& command);
    void Init(void);
};
