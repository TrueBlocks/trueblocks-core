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
    bool json_on;
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
    bool exportData               (void);

    bool     loadWatches    (const CToml& toml);
    string_q annotate       (const string_q& strIn) const;

    typedef bool (*NAMEVISITFUNC)(string_q& str, const CAccountWatch& watch);

    void displayTransaction (ostream& os, const CTransaction *theTrans) const;
    void displayTrace (ostream& os, const CTransaction *theTrans) const;
    void renameItems(string_q& str, const CAccountWatchArray& array) const;
};

//------------------------------------------------------------------------
extern bool articulateTransaction(CTransaction *p);
extern bool articulateEvent(CLogEntry *p);

//-----------------------------------------------------------------------------
extern CStringArray signatures;
extern CStringArray topics;
