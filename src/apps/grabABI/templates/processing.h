#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
/*
 * This code was generated automatically from grabABI and makeClass. You may
 * edit the file, but keep your edits inside the 'EXISTING_CODE' tags.
 */

//EXISTING_CODE
//EXISTING_CODE

//-----------------------------------------------------------------------
#include "etherlib.h"
#include "debug.h"

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    SFString mode;
    bool single_on;
    bool accounting_on;
    bool logs_on;
    bool trace_on;
    bool bloom_on;
    bool debugger_on;
    bool parse_on;
    bool autocorrect_on;
    blknum_t kBlock;

    COptions(void);
    ~COptions(void);

    bool parseArguments(SFString& command);
    void Init(void);
};

//-----------------------------------------------------------------------
class CBlockStats {
public:
    blknum_t nBlocks;
    blknum_t firstBlock;
    blknum_t lastBlock;
    blknum_t minWatchBlock;
    blknum_t maxWatchBlock;
    CBlock   prevBlock;
    CBlockStats(void) :
        nBlocks(0),
        firstBlock(0),
        lastBlock(0),
        minWatchBlock(0),
        maxWatchBlock(UINT32_MAX),
        prevBlock()
    {  }
};

//-----------------------------------------------------------------------
class CTransStats {
public:
    uint64_t nAccountedFor;
    uint64_t nDisplayed;
    uint64_t nFreshened;
    CTransStats(void) :
        nAccountedFor(0),
        nDisplayed(0),
        nFreshened(0)
    {  }
};

//-----------------------------------------------------------------------
class CBloomStats {
public:
    blknum_t bloomsChecked;
    blknum_t bloomHits;
    blknum_t falsePositives;
    CBloomStats(void) :
        bloomsChecked(0),
        bloomHits(0),
        falsePositives(0)
    {  }
};

//-----------------------------------------------------------------------
class CVisitor {
public:
    COptions     opts;
    CBloomStats  bloomStats;
    CTransStats  transStats;
    CBlockStats  blockStats;
    CTransBuffer tBuffer;

    CAccountWatchArray watches;
    CAccountWatchArray named;

    SFArchive cache;
    SFString screenFmt;
    bool esc_hit;
    bool user_hit_q;

    CVisitor(void) :
        opts(), bloomStats(), transStats(), blockStats(), tBuffer(),
//      watches, named,
        cache(READING_ARCHIVE),
        screenFmt(""), esc_hit(false), user_hit_q(false)
    { barLen(80); }

    bool isTransactionOfInterest  (CTransaction *trans, uint32_t& which);

    bool openIncomeStatement      (const CBlock& block);
    bool accountForExtTransaction (const CBlock& block, const CTransaction *trans);
    bool accountForIntTransaction (const CBlock& block, const CTransaction *trans, const CTrace *trace);
    bool closeIncomeStatement     (const CBlock& block);
    bool enterDebugger            (const CBlock& block);

    blknum_t loadWatches(const CToml& toml);
    SFString annotate   (const SFString& strIn) const;

    void displayTrans (const CTransaction *theTrans) const;
    void displayTrace (timestamp_t ts, const CTraceArray& traces, bool err) const;
    void displayBloom (const SFBloom& bloom, const SFString& msg, const SFString& res) const;
};

//-----------------------------------------------------------------------
extern bool displayFromCache      (const SFString& fileName, SFUint32& blockNum, void *dataPtr);
extern bool updateCacheUsingBlooms(const SFString& str, void *dataPtr);
extern bool updateCache           (CBlock& block, void *dataPtr);
extern void myQuitHandler         (int s);
inline void myOnExitHandler       (void) { myQuitHandler(1); }

//EXISTING_CODE
//EXISTING_CODE
