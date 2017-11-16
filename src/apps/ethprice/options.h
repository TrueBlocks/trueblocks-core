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
    bool        freshen;
    uint32_t    freq;
    uint32_t    hour;
    timestamp_t at;

    COptions(void);
    ~COptions(void) {};

    bool parseArguments(SFString& command) override;
    void Init(void) override;
    SFString postProcess(const SFString& which, const SFString& str) const override;
};
