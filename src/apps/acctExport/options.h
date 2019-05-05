#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "acctlib.h"

//-----------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    CAccountWatchArray monitors;
    CAppearanceArray_base items;
    blknum_t lastAtClient;
    bool writeBlocks;
    bool writeTrxs;
    bool writeTraces;
    bool skipDdos;
    size_t maxTraces;
    export_t fmt;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);
    string_q postProcess(const string_q& which, const string_q& str) const;
};
