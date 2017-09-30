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
    bool isCheck;
    bool isRaw;
    bool asks4Cache;
    bool terse;
    bool force;
    bool normalize;
    bool silent;
    int quiet;
    COptionsBlockList blocks;

    COptions(void);
    ~COptions(void);

    bool parseArguments(SFString& command);
    void Init(void);
    bool isMulti(void) const;
};

//-----------------------------------------------------------------------------
extern SFString doOneBlock(SFUint32 num, const COptions& opt);
extern SFString checkOneBlock(SFUint32 num, const COptions& opt);
extern SFString normalizeBlock(const SFString& inIn, bool remove);
extern SFString diffStr(const SFString& str1, const SFString& str2);
extern void interumReport(ostream& os, blknum_t i);
