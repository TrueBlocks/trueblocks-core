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
#include "debug.h"

//-----------------------------------------------------------------------
class CVisitor {
public:
    SFArchive cache;
    SFString screenFmt;
    SFString logFmt;
    SFString traceFmt;
    SFString color;
    SFString hiColor;
    SFString hiColor2;
    bool notify;
    bool accounting_on;
    bool debugger_on;
    bool logs_on;
    bool trace_on;
    bool bloom_on;
    bool single_on;
    bool parse_on;
    SFUint32 nAccountedFor;
    SFUint32 nDisplayed;
    SFUint32 nFreshened;
    bool cacheOnly;
    bool autoTrace;
    bool autoCorrect;
    CTransBuffer tBuffer;
    SFUint32 visited;
    SFUint32 nBlocksToVisit;
    SFUint32 startBlock;
    SFUint32 endBlock;
    timestamp_t last_ts;
    uint32_t lastWhich;
    const CTransaction *lastTrans;

    CVisitor(void) : cache(true, NO_SCHEMA, false),
        screenFmt(""), notify(false), accounting_on(false), debugger_on(false), logs_on(false),
        trace_on(false), bloom_on(false), single_on(false), parse_on(false), nAccountedFor(0),
        nDisplayed(0), nFreshened(0), cacheOnly(false), autoTrace(false), autoCorrect(false),
        tBuffer(), visited(0), nBlocksToVisit(0), startBlock(0), endBlock(0), last_ts(0),
        lastWhich((uint32_t)NOPOS), lastTrans(NULL) { barLen(80); }

    void setColors(const CTransaction *trans, const SFString& contractColor) {
        color    = (trans->isError ? biBlack : SFString(trans->isInternalTx ? cRed : contractColor));
        hiColor  = (trans->isError ? biBlack : cWhite);
        hiColor2 = (trans->isError ? biBlack : cTeal);
    }

    bool ofInterest(CTransaction *trans, uint32_t& which);

    void openIncomeStatement(const CBlock& block);
    void accountForExtTransaction(const CBlock& block, const CTransaction *trans);
    void accountForIntTransaction(const CBlock& block, const CTrace *trace);
    bool closeIncomeStatement(const CBlock& block);
    SFUint32 nProcessed(void) const {
        return nDisplayed + nFreshened + nAccountedFor;
    }
    CAccountWatchArray watches;
    bool enterDebugger(const CBlock& block);
    void initReport(void) const {
        cerr << startBlock << " : " << endBlock << " : " << nBlocksToVisit << " : " << visited << "\r\n";
        cerr.flush();
    }

    void interumReport1(SFUint32 bn, timestamp_t ts) {
        cerr << "fromCache: " << bn << " " << dateFromTimeStamp(ts) << "\r";
        cerr.flush();
        last_ts = ts;
    }

    void interumReport(SFUint32 bn, timestamp_t ts, const SFString& endMsg) {
        progressBar(bn-startBlock, nBlocksToVisit, endMsg);
        last_ts = ts;
    }

    void showColoredTrace(const SFHash& hash, bool err);
};

//-----------------------------------------------------------------------
extern bool displayFromCache(const SFString& fileName, SFUint32& blockNum, void *data);
extern bool updateCache(CBlock& block, void *data);

//EXISTING_CODE
extern bool displayTransaction(uint32_t which, const CTransaction *theTrans, void *data);
extern void showColoredBloom(const SFBloom& bloom, const SFString& msg, const SFString& res);
//EXISTING_CODE
