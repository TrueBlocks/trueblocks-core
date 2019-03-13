#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "acctlib.h"

//-----------------------------------------------------------------------------
class CStats {
public:
    blknum_t nSeen;
    blknum_t nSkipped;
    blknum_t nHit;
    CStats(void) : nSeen(0), nSkipped(0), nHit(0) { }
};
#define nQueried nSkipped

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
public:

    CApiSpec           api_spec;
    blknum_t           startScrape;
    blknum_t           scrapeCnt;

    timestamp_t        lastTimestamp;

    uint64_t           logLevel;
    bool               ignoreBlooms;
    bool               ignoreBlkCache;
    bool               writeBlocks;
    bool               useIndex;

    string_q           exclusions;

    CStats             blkStats;
    CStats             addrStats;
    CStats             transStats;
    CStats             traceStats;

    CAccountWatchArray monitors;
    CAccountWatch      primary;

    bool               blockCounted;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);
    bool isExcluded(const address_t& addr) { return contains(exclusions, addr); }
    friend ostream& operator<<(ostream& os, const COptions& item);
    void showBanner(void) const;
    string_q finalReport(double timing, bool header) const;
    bool finalReport(void) const;
    bool foundAHit(const CAccountWatch *acct, const CBlock& block, const CTransaction *trans);
};

//-----------------------------------------------------------------------
extern bool processBlock  (blknum_t bn, COptions *options);
extern bool processTrans  (const CBlock& block, const CTransaction *trans, COptions *options);
extern bool processTraces (const CBlock& block, const CTransaction *trans, const CAccountWatch *acct, COptions *options);

//-----------------------------------------------------------------------
extern bool visitIndexFiles  (const string_q& path, void *data);
extern bool visitBloomFilters(const string_q& path, void *data);
