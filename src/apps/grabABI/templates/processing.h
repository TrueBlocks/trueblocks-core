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
    blknum_t kBlock;

    COptions(void);
    ~COptions(void);

    bool parseArguments(SFString& command);
    void Init(void);
};

//-----------------------------------------------------------------------
class CBloomStats {
public:
    uint32_t bloomsChecked;
    uint32_t bloomHits;
    uint32_t falsePositives;
    double startTime;
    CBloomStats(void) : bloomsChecked(0), bloomHits(0), falsePositives(0) { startTime = qbNow(); }
};

//-----------------------------------------------------------------------
class CVisitor {
public:
    COptions opts;
    SFArchive cache;
    SFString screenFmt;
    SFString verboseFmt;
    bool esc_hit;
    SFUint32 nAccountedFor;
    SFUint32 nDisplayed;
    SFUint32 nFreshened;
    bool autoCorrect;
    CTransBuffer tBuffer;
    SFUint32 nBlocksToVisit;
    SFUint32 startBlock;
    SFUint32 endBlock;
    timestamp_t last_ts;
    const CTransaction *lastTrans;
    blknum_t lastBlockNum;
    SFUint32 nOutOfBal;
    blknum_t minWatchBlock;
    blknum_t maxWatchBlock;
    CBloomStats bloomStats;
    CAccountWatchArray watches;
    CAccountWatchArray named;

    CVisitor(void) :
        cache(true, NO_SCHEMA, false),
        screenFmt(""), verboseFmt(""),
        esc_hit(false), nAccountedFor(0),
        nDisplayed(0), nFreshened(0),
        autoCorrect(false),
        tBuffer(), nBlocksToVisit(0), startBlock(0), endBlock(0), last_ts(0),
        lastTrans(NULL), lastBlockNum(0), nOutOfBal(0),
        minWatchBlock(0), maxWatchBlock(UINT32_MAX) { barLen(80); }

    bool isTransactionOfInterest(CTransaction *trans, uint32_t& which);

    bool openIncomeStatement(const CBlock& block);
    bool accountForTransaction(const CBlock& block, const CTransaction *trans);
    bool accountForIntTransaction(const CBlock& block, const CTransaction *trans, const CTrace *trace);
    bool closeIncomeStatement(const CBlock& block);
    blknum_t loadWatches(const CToml& toml);
    SFUint32 nProcessed(void) const {
        return nDisplayed + nFreshened + nAccountedFor;
    }
    bool enterDebugger(const CBlock& block);

    void interumReport1(SFUint32 bn, timestamp_t ts) {
        cerr << "fromCache: " << bn << " " << dateFromTimeStamp(ts) << "\r";
        cerr.flush();
        last_ts = ts;
    }

    void interumReport(SFUint32 bn, timestamp_t ts, const SFString& endMsg) {
        blknum_t x = (startBlock >= bn ? 0 : bn - startBlock);
        progressBar(x, nBlocksToVisit, endMsg);
        last_ts = ts;
    }

    void showColoredTrace(timestamp_t ts, const CTraceArray& traces, bool err) const;
    bool displayTransaction(const CTransaction *theTrans) const;
    void showColoredBloom(const SFBloom& bloom, const SFString& msg, const SFString& res) const;
    SFString annotate(const SFString& strIn) const;
};

//-----------------------------------------------------------------------
extern bool displayFromCache(const SFString& fileName, SFUint32& blockNum, void *data);
extern bool updateCache(CBlock& block, void *data);
extern bool updateCacheUsingBlooms(const SFString& str, void *data);
extern void myQuitHandler(int s);
inline void myOnExitHandler(void) { myQuitHandler(1); }

//EXISTING_CODE
//EXISTING_CODE
