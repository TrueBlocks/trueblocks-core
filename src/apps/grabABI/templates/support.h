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
class CStats {
public:
    SFUint32 visited;
    SFUint32 nBlocks;
    SFUint32 startBlock;
    SFUint32 endBlock;
    timestamp_t last_ts;

    CStats(SFUint32 start, SFUint32 end) : visited(0), nBlocks(end-start), startBlock(start),
            endBlock(end), last_ts(0) { barLen(80); }

    void initReport(void) const {
        cerr << startBlock << " : " << endBlock << " : " << nBlocks << " : " << visited << "\r\n";
        cerr.flush();
    }

    void interumReport1(SFUint32 bn, timestamp_t ts) {
        cerr << "fromCache: " << bn << " " << dateFromTimeStamp(ts) << "\r";
        cerr.flush();
        last_ts = ts;
    }

    void interumReport(SFUint32 bn, timestamp_t ts) {
        timestamp_t tsOut = (ts == 0 ? toTimeStamp(Now()) : ts);
        progressBar(bn-startBlock, nBlocks, dateFromTimeStamp(tsOut).Format(FMT_JSON));
        last_ts = ts;
    }
};

//-----------------------------------------------------------------------
class CVisitor {
public:
    SFArchive cache;
    SFString screenFmt;
    SFString color;
    SFString hiColor;
    SFString hiColor2;
    CStats stats;
    bool useColor;
    bool toScreen;
    bool showLogs;
    bool notify;
    bool accounting;
    bool debug;
    blknum_t breakPt;
    SFUint32 nTrans;
    bool cacheOnly;
    bool autoTrace;
    CTransBuffer tBuffer;
    CVisitor(void) : cache(true, NO_SCHEMA, false),
        screenFmt(""), stats(0,0), useColor(true), toScreen(true),
        showLogs(false), notify(false), accounting(false), debug(false),
        breakPt(UINT64_MAX), nTrans(false), cacheOnly(false),
        autoTrace(true), tBuffer() { }

    void setColors(const CTransaction *trans, const SFString& contractColor) {
        color    = (trans->isError ? biBlack : SFString(trans->isInternalTx ? cRed : contractColor));
        hiColor  = (trans->isError ? biBlack : cWhite);
        hiColor2 = (trans->isError ? biBlack : cTeal);
    }

    bool ofInterest(CTransaction *trans, uint32_t& which);

    void openIncomeStatement(const CBlock& block);
    void accountForTransaction(const CTransaction *trans);
    bool closeIncomeStatement(const CBlock& block);

    CAccountWatchArray watches;
    bool enterDebugger(void);
};

//-----------------------------------------------------------------------
extern int  displayCache(const SFString& fileName, SFUint32& blockNum, void *data);
extern bool buildCache(CBlock& block, void *data);

//EXISTING_CODE
extern void displayTransaction(uint32_t which, const CTransaction *theTrans, void *data);
//EXISTING_CODE
