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
    CBlockStats(void) :
        nBlocks(0),
        firstBlock(0),
        lastBlock(0),
        minWatchBlock(0),
        maxWatchBlock(UINT32_MAX)
    {  }
};

//-----------------------------------------------------------------------
class CTransStats {
public:
    uint64_t nAccountedFor;
    uint64_t nDisplayed;
    CTransStats(void) :
        nAccountedFor(0),
        nDisplayed(0)
    {  }
};

//-----------------------------------------------------------------------
class CVisitor {
public:
    COptions     opts;
    CTransStats  transStats;
    CBlockStats  blockStats;
    CTransBuffer tBuffer;

    CAccountWatchArray watches;
    CAccountWatchArray named;

    SFArchive cache;
    SFString screenFmt;
    bool esc_hit;

    CVisitor(void) :
        opts(), transStats(), blockStats(), tBuffer(),
//      watches, named,
        cache(READING_ARCHIVE),
        screenFmt(""), esc_hit(false)
    { barLen(80); }

    bool openIncomeStatement      (const CBlock& block);
    bool accountForExtTransaction (const CBlock& block, const CTransaction *trans);
    bool accountForIntTransaction (const CBlock& block, const CTransaction *trans, const CTrace *trace);
    bool closeIncomeStatement     (const CBlock& block);
    bool enterDebugger            (const CBlock& block);

    blknum_t loadWatches    (const CToml& toml);
    SFString annotate       (const SFString& strIn) const;
    CAccountWatch *findWatch(SFAddress addr);
    CAccountWatch *findWatch(uint32_t which);
    uint32_t checkForImport (void);
    bool hasImport(void);

    void displayTrans (uint32_t which, const CTransaction *theTrans) const;
    void displayTrace (timestamp_t ts, const CTraceArray& traces, bool err) const;
};

//-----------------------------------------------------------------------
extern bool displayFromCache      (const SFString& fileName, uint64_t& blockNum, void *dataPtr);
extern void myQuitHandler         (int s);
inline void myOnExitHandler       (void) { myQuitHandler(1); }

//EXISTING_CODE
//EXISTING_CODE
