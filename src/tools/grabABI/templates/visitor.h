#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
/*
 * This code was generated automatically from grabABI and makeClass. You may
 * edit the file, but keep your edits inside the 'EXISTING_CODE' tags.
 */

// EXISTING_CODE
// EXISTING_CODE

//-----------------------------------------------------------------------
#include "etherlib.h"
#include "options.h"
#include "debug.h"

//-----------------------------------------------------------------------
class CBlockStats {
  public:
    blknum_t nBlocks;
    blknum_t firstBlock;
    blknum_t lastBlock;
    blknum_t minWatchBlock;
    blknum_t maxWatchBlock;
    CBlockStats(void) : nBlocks(0), firstBlock(0), lastBlock(0), minWatchBlock(0), maxWatchBlock(UINT32_MAX) {
    }
};

//-----------------------------------------------------------------------
class CTransStats {
  public:
    uint64_t nAccountedFor;
    uint64_t nDisplayed;
    CTransStats(void) : nAccountedFor(0), nDisplayed(0) {
    }
};

//-----------------------------------------------------------------------
class COptions : public COptionsBase {
  public:
    bool single_on;
    bool no_check;
    bool accounting_on;
    bool logs_on;
    bool trace_on;
    bool debugger_on;
    bool export_on;
    bool parse_on;
    bool autocorrect_on;
    bool json_on;
    bool report_bals;
    blknum_t kBlock;
    blknum_t offset;
    uint64_t theWidth;
    bool parseArguments(string_q& command);
    void Init(void);

    CTransStats transStats;
    CBlockStats blockStats;
#ifdef DEBUGGER_ON
    CTransBuffer tBuffer;
#endif

    CAccountWatchArray monitors;
    CAccountWatchArray named;

    string_q transFmt;
    string_q traceFmt;
    bool esc_hit;

    COptions(void);
    ~COptions(void);

    bool openIncomeStatement(const CBlock& block);
    bool accountForExtTransaction(const CBlock& block, const CTransaction* trans);
    bool accountForIntTransaction(const CBlock& block, const CTransaction* trans, const CTrace* trace);
    bool closeIncomeStatement(const CBlock& block);
    bool enterDebugger(const CBlock& block);
    bool displayFromCache(uint64_t startBlock);

    bool loadWatches(const CToml& toml);
    string_q annotate(const string_q& strIn) const;

    void displayTrans(ostream& os, const CTransaction* theTrans) const;
    void displayTrace(ostream& os, const CTransaction* theTrans) const;
    typedef bool (*NAMEVISITFUNC)(string_q& str, const CAccountWatch& watch);
    void renameItems(string_q& str, const CAccountWatchArray& array) const;
};

// EXISTING_CODE
// EXISTING_CODE
