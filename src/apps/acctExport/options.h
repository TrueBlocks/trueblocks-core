#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "acctlib.h"
#include "tokenlib.h"
#include "walletlib.h"

//-----------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    address_t filter;
    blknum_t blk_minWatchBlock;
    blknum_t blk_maxWatchBlock;
    string_q defaultFmt;
    string_q hideFields;
    string_q showFields;
    string_q fmtStr;
    CAccountWatchArray watches;
    CAccountWatchArray named;
    CAcctCacheItemArray items;
    CAddressAppearanceArray addrsInBlock;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);

    bool     loadWatches    (const CToml& toml);
    string_q annotate       (const string_q& strIn) const;

    typedef bool (*NAMEVISITFUNC)(string_q& str, const CAccountWatch& watch);
    void renameItems(string_q& str, const CAccountWatchArray& array) const;
};

//------------------------------------------------------------------------
extern bool loadData(COptions& options);
extern bool exportData(COptions& options);
extern bool articulateTransaction(CTransaction *p);
extern bool articulateEvent(CLogEntry *p);

//-----------------------------------------------------------------------------
extern CStringArray signatures;
extern CStringArray topics;
