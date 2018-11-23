#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "acctlib.h"

//#define OUTPUT_REDIR 1
#ifdef OUTPUT_REDIR
class COutputPipe {
public:
    COutputPipe(streambuf * buf, ostream& os) : redir(os) {
        old_buf = os.rdbuf(buf);
    }

    ~COutputPipe(void) {
        redir.rdbuf(old_buf);
    }
    ostream& redir;
    streambuf *old_buf;
};
#endif

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
    bool ignoreDdos;
#ifdef OUTPUT_REDIR
    COutputPipe *out;
    string_q outFile;
    ofstream outStream;
#endif

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);

    bool     loadWatches    (const CToml& toml);
    string_q annotate       (const string_q& strIn) const;
    bool     shouldTrace    (const CTransaction *trans) const;

    typedef bool (*NAMEVISITFUNC)(string_q& str, const CAccountWatch& watch);
    void renameItems(string_q& str, const CAccountWatchArray& array) const;
};

//------------------------------------------------------------------------
extern bool loadData(COptions& options);
extern bool exportData(COptions& options);

//------------------------------------------------------------------------
extern string_q cleanFmt    (const string_q& str);
extern string_q defTransFmt;

//-----------------------------------------------------------------------------
extern CStringArray signatures;
extern CStringArray topics;

//-----------------------------------------------------------------------------
enum export_t {
    JSON = 1,
    TXT = 2,
    CSV = 3,
};
