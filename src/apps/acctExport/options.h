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

class COutPiped {
public:
    COutPiped(streambuf * buf, ostream& os)
        : redir(os) {
            old_buf = os.rdbuf(buf);
        }
    ~COutPiped(void)
        {
            redir.rdbuf(old_buf);
        }
    ostream& redir;
    streambuf *old_buf;
};

//-----------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    blknum_t blk_minWatchBlock;
    blknum_t blk_maxWatchBlock;
    string_q transFmt;
    CAccountWatchArray watches;
    CAccountWatchArray named;
    CAcctCacheItemArray items;
    CBlock curBlock;
    bool useBloom;
    bool needsArt;
    bool needsTrace;
    bool showProgress;
    COutPiped *out;
    string_q outFile;
    ofstream outStream;

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
extern bool articulateTransaction(const CAbi& abi, CTransaction *p);
extern bool articulateEvent(const CAbi& abi, CLogEntry *l);
extern bool articulateTrace(const CAbi& abi, CTrace *t);

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
