#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"

#define MAX_ADDRS 40
class COptions : public COptionsBase {
public:
    SFUint32 parts;
    bool noconst;
    bool open;
    SFAddress addrs[MAX_ADDRS+2];
    SFUint32 nAddrs;
    SFString primaryAddr;
    SFString classDir;
    SFString prefix;
    SFString theABI;
    bool silent;

    COptions(void);
    ~COptions(void);

    bool parseArguments(SFString& command);
    void Init(void);
    bool isToken(void) const { return prefix == "tokenlib"; }
    bool isWallet(void) const { return prefix == "walletlib"; }
    bool isBuiltin(void) const { return isToken() || isWallet(); }
};

extern SFString getPrefix(const SFString& in);
