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

#define cBlack 1
#define cRed 1
#define cGreen 1
#define cYellow 1
#define cBlue 1
#define cMagenta 1
#define cTeal 1
#define cWhite 1
#define cOff 1
#define bbold 1
#define italic 1
#define underline 1
#define reversed 1
#define strikethru 1
#define greenCheck 1
#define redX 1
#undef biBlack
#undef bBlack
#undef bRed
#undef bGreen
#undef bYellow
#undef bBlue
#undef bMagenta
#undef bTeal
#undef bWhite
#undef iBlack
#undef iRed
#undef iGreen
#undef iYellow
#undef iBlue
#undef iMagenta
#undef iTeal
#undef iWhite
#undef asYellow

//-----------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    blknum_t blk_nBlocks;
    blknum_t blk_firstBlock;
    blknum_t blk_lastBlock;
    blknum_t blk_minWatchBlock;
    blknum_t blk_maxWatchBlock;
    uint64_t tx_nAccountedFor;
    uint64_t tx_nDisplayed;
    blknum_t theStartBlock;
    bool accounting_on;
    bool logs_on;
    bool trace_on;
    bool autocorrect_on;
    string_q defaultFmt;
    bool report_bals;
    string_q hideFields;
    string_q showFields;
    blknum_t kBlock;
    bool parseArguments(string_q& command);
    void Init(void);

    CAccountWatchArray watches;
    CAccountWatchArray named;

    string_q transFmt;
    string_q traceFmt;

    COptions(void);
    ~COptions(void);

    bool openIncomeStatement      (const CBlock& block);
    bool accountForExtTransaction (const CBlock& block, const CTransaction *trans);
    bool accountForIntTransaction (const CBlock& block, const CTransaction *trans, const CTrace *trace);
    bool closeIncomeStatement     (const CBlock& block);

    bool     loadWatches    (const CToml& toml);
    string_q annotate       (const string_q& strIn) const;

    typedef bool (*NAMEVISITFUNC)(string_q& str, const CAccountWatch& watch);

    void displayTransaction (ostream& os, const CTransaction *theTrans) const;
    void displayTrace (ostream& os, const CTransaction *theTrans) const;
    void renameItems(string_q& str, const CAccountWatchArray& array) const;
};

//------------------------------------------------------------------------
extern bool upgradeData(COptions& options);
extern bool exportData(COptions& options);
extern bool articulateTransaction(CTransaction *p);
extern bool articulateEvent(CLogEntry *p);

//-----------------------------------------------------------------------------
extern CStringArray signatures;
extern CStringArray topics;
