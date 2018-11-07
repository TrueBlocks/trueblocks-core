#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "etherlib.h"
#include "acctlib.h"
#include "tokenlib.h"
#include "walletlib.h"

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
