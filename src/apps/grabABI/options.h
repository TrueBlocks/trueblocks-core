#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"

#define MAX_ADDRS 400
class COptions : public COptionsBase {
public:
    uint64_t parts;
    bool noconst;
    bool open;
    bool asJson;
    bool raw;
    bool asData;
    bool decNames;
    SFAddress addrs[MAX_ADDRS+2];
    uint64_t nAddrs;
    SFString primaryAddr;
    SFString classDir;
    SFString prefix;
    SFString theABI;
    bool silent;

    COptions(void);
    ~COptions(void);

    SFString postProcess(const SFString& which, const SFString& str) const override;
    bool parseArguments(SFString& command) override;
    void Init(void) override;

    bool isToken(void) const { return prefix == "tokenlib"; }
    bool isWallet(void) const { return prefix == "walletlib"; }
    bool isBuiltin(void) const { return isToken() || isWallet(); }
};

extern SFString getPrefix(const SFString& in);
