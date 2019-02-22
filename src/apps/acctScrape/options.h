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
    blknum_t           lastBlock;
    blknum_t           minWatchBlock;
    blknum_t           maxWatchBlock;
    blknum_t           maxBlocks;
    timestamp_t        lastTimestamp;
    bool               ignoreBlooms;
    bool               ignoreBlkCache;
    bool               writeBlocks;
    bool               useIndex;
    bool               isList;
    string_q           exclusions;
    blknum_t           firstBlock;
    blknum_t           nBlocks;
    CStats             blkStats;
    CStats             addrStats;
    CStats             transStats;
    CStats             traceStats;
    bool               blockCounted;
    CAccountWatchArray monitors;
    CArchive           txCache;
    string_q           name;
    uint64_t           logLevel;
    CToml             *toml;
    string_q           cacheFilename;
    address_t          monitorAddr;
    blknum_t           lastInCache;
    blknum_t           lastVisited;
    string_q           ex_data;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);
    bool isExcluded(const address_t& addr) { return contains(exclusions, addr); }
    bool loadMonitors(void);
    friend ostream& operator<<(ostream& os, const COptions& item);
    string_q finalReport(double timing, bool header) const;
    bool shouldScrape(void) const;
    bool finalReport(void) const;
    bool foundOne(const CAccountWatch *acct, const CBlock& block, const CTransaction *trans);
    void writeLastBlock(blknum_t bn);
};

//-----------------------------------------------------------------------
extern bool processBlock  (blknum_t bn, COptions *options);
extern bool processTrans  (const CBlock& block, const CTransaction *trans, COptions *options);
extern bool processTraces (const CBlock& block, const CTransaction *trans, const CAccountWatch *acct, COptions *options);

//-----------------------------------------------------------------------
extern bool visitIndexFiles  (const string_q& path, void *data);
extern bool visitBloomFilters(const string_q& path, void *data);
