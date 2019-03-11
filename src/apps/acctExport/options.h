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
    CAccountWatchArray named;
    CAcctCacheItemArray items;
    CBlock curBlock;
    bool useBloom;
    bool needsArt;
    bool needsTrace;
    bool showProgress;
    bool ignoreDdos;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);
    string_q postProcess(const string_q& which, const string_q& str) const;

    string_q annotate       (const string_q& strIn) const;
    bool     shouldTrace    (const CTransaction *trans) const;

    typedef bool (*NAMEVISITFUNC)(string_q& str, const CAccountWatch& watch);
    void renameItems(string_q& str, const CAccountWatchArray& array) const;
};

//------------------------------------------------------------------------
extern bool loadData(COptions& options);
extern bool exportData(COptions& options);

//-----------------------------------------------------------------------------
enum export_t {
    JSON = 1,
    TXT = 2,
    CSV = 3,
};
