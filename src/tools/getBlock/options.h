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
    bool isCache;
    bool terse;
    bool force;
    bool normalize;
    bool silent;
    int quiet;
    SFString format;
    bool priceBlocks;
    COptionsBlockList blocks;

    COptions(void);
    ~COptions(void);

    SFString postProcess(const SFString& which, const SFString& str) const override;
    bool parseArguments(SFString& command) override;
    void Init(void) override;
    bool isMulti(void) const;
    SFString getBlockNumList(void) const;
};

//-----------------------------------------------------------------------------
extern SFString doOneBlock(uint64_t num, const COptions& opt);
extern SFString checkOneBlock(uint64_t num, const COptions& opt);
extern SFString normalizeBlock(const SFString& inIn, bool remove, bool isByzan);
extern SFString diffStr(const SFString& str1, const SFString& str2);
extern void interumReport(ostream& os, blknum_t i);
