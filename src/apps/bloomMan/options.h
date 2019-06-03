#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"

class COptions;
//-------------------------------------------------------------------------
class CStatistics {
public:
    const COptions *options;
    bool            cummulative;
    blknum_t        curBucket;
    blknum_t        lastReport;
    uint64_t        nVisits, nFiles, nEmpty;
    uint64_t        nBlooms, nBits, nBytes;

    CStatistics(const COptions *opts) : options(opts) { Reset(); }
    void Reset(void) {
        curBucket = NOPOS;
        lastReport = NOPOS;
        nVisits = nFiles = nEmpty = 0;
        nBlooms = nBits = nBytes = 0;
    }
    string_q interumReport(blknum_t bn) const;
    string_q report(blknum_t bn) const;
};

//-------------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    COptionsBlockList blocks;
    string_q          address_list;
    bool              isStats;
    bool              isRewrite;
    bool              isCheck;
    bool              hasAddrs;
    bool              hasBlocks;
    uint64_t          bucketSize;
    uint64_t          bitBound;
    CStatistics       stats;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);
};

//-----------------------------------------------------------------------------
extern bool handle_visit  (blknum_t bn, void *data);
extern bool handle_rewrite(blknum_t bn, void *data);
extern bool handle_stats  (blknum_t bn, void *data);
extern bool handle_check  (blknum_t bn, void *data);
extern bool handle_addrs  (blknum_t bn, void *data);
extern bool handle_raw    (blknum_t bn, void *data);

//-----------------------------------------------------------------------------
extern bool accumAddrs (const CAppearance& item, void *data);
extern bool transFilter(const CTransaction *trans, void *data);
