#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "acctlib.h"
#include "tokenlib.h"
#include "walletlib.h"

//-----------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    blknum_t blk_minWatchBlock;
    blknum_t blk_maxWatchBlock;
    string_q transFmt;
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
    void displayTransaction(ostream& os, const CTransaction *theTrans) const;
    void displayTrace(ostream& os, const CTransaction *theTrans) const;
};

//------------------------------------------------------------------------
extern bool loadData(COptions& options);
extern bool exportData(COptions& options);
extern bool articulateTransaction(CTransaction *p);
extern bool articulateEvent(CLogEntry *p);

//------------------------------------------------------------------------
extern string_q cleanFmt    (const string_q& str);
extern void     manageFields(const string_q& listIn, bool show);
extern string_q defTransFmt;
extern string_q defHide;
extern string_q defShow;

//-----------------------------------------------------------------------------
extern CStringArray signatures;
extern CStringArray topics;

//-----------------------------------------------------------------------------
enum export_t {
    JSON = 1,
    TXT = 2,
    CSV = 3,
};
