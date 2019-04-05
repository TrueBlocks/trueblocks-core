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

    bool               new_version;
    
    CApiSpec           api_spec;
    blknum_t           startScrape;
    blknum_t           scrapeCnt;

    timestamp_t        lastTimestamp;

    uint64_t           logLevel;
    bool               ignoreBlooms;
    bool               ignoreBlkCache;
    bool               useBlooms;

    CStats             blkStats;
    CStats             addrStats;
    CStats             transStats;
    CStats             traceStats;

    CAccountWatchArray monitors;
    CAccountWatch      primary;

    bool               blockCounted;
    blknum_t           firstBlockInFile;
    blknum_t           lastBlockInFile;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);
    friend ostream& operator<<(ostream& os, const COptions& item);
    string_q finalReport(double timing, bool header) const;
    bool finalReport(void) const;
    bool foundAHit(const CAccountWatch *acct, const CBlock& block, const CTransaction *trans);
    bool visitBinaryFile(const string_q& path, void *data);
    bool visitAsciiFile(const string_q& path, void *data);
    bool processTooEarlyFile(const string_q& path);
};

//-----------------------------------------------------------------------
extern bool processBlock  (blknum_t bn, COptions *options);
extern bool processTrans  (const CBlock& block, const CTransaction *trans, COptions *options);
extern bool processTraces (const CBlock& block, const CTransaction *trans, const CAccountWatch *acct, COptions *options);

//-----------------------------------------------------------------------
extern bool visitBloomFilters(const string_q& path, void *data);
extern bool visitIndexFiles  (const string_q& path, void *data);

//----------------------------------------------------------------
struct AddressRecord_ondisc {
    uint8_t bytes[20];
    uint32_t offset;
    uint32_t cnt;
};

//----------------------------------------------------------------
struct BlockRecord_ondisc {
    uint32_t blk;
    uint32_t txid;
};

//----------------------------------------------------------------
inline int findRecord(const void* v1, const void* v2) {
    const AddressRecord_ondisc *at1 = (AddressRecord_ondisc*)v1;
    const AddressRecord_ondisc *at2 = (AddressRecord_ondisc*)v2;
    for (size_t i = 0 ; i < 20 ; i++) {
        int ret = at1->bytes[i] - at2->bytes[i];
        if (ret)
            return ret;
    }
    return 0;
}

//----------------------------------------------------------------
struct Header_ondisc {
    uint32_t magic;
    uint8_t hash[32];
    uint32_t nAddrs;
    uint32_t nRows;
};

//----------------------------------------------------------------
struct CIndexRecord {
public:
    char addr  [43];  // '0x' + 40 chars + \t
    char block [10];  // less than 1,000,000,000 + \t
    char txid  [6];   // less than 100,000 + \n
};

//---------------------------------------------------------------
inline int findFunc(const void *v1, const void *v2) {
    const CIndexRecord *t1 = (const CIndexRecord *)v1;
    const CIndexRecord *t2 = (const CIndexRecord *)v2;
    return strncmp(t1->addr, t2->addr, 42);
}
