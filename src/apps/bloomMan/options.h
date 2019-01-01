#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"

//-------------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    bool              isStats;
    bool              isReWrite;
    bool              isCheck;
    bool              isCummulative;
    bool              isRaw;
    bool              hasAddrs;
    bool              hasBlocks;
    uint64_t          bucketSize;
    COptionsBlockList blocks;
    string_q          address_list;
    uint64_t          bitBound;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);
};

//-------------------------------------------------------------------------
#define sep '\t'

//-------------------------------------------------------------------------
class CStatistics {
public:
    COptions options;
    blknum_t lastMarker;
    uint64_t nBlocks, nWithBlooms, nWithoutBlooms;
    uint64_t nBlooms, nBits, nBytes;

    CStatistics(const COptions& opts) : options(opts) { Reset(); }

    void Reset(void) {
        lastMarker = 0;
        nBlocks = nWithBlooms = nWithoutBlooms = 0;
        nBlooms = nBits = nBytes = 0;
    }

    void report(blknum_t bn) const {
        cout << string_q(90, ' ') << "\r";
        static bool here = false;
        if (!here) {
            here = true;
            cout << "blockNum" << sep;
            cout << "nBlocks" << sep << "nBloomFiles" << sep << "noBlooms" << sep;
            cout << "nBlooms" << sep << "nBits" << sep << "nBytes" << "\n";
        }
        cout << bn << sep;
        cout << nBlocks << sep << nWithBlooms << sep << nWithoutBlooms << sep;
        cout << nBlooms << sep << nBits << sep << nBytes << "\n";
        cout.flush();
    }

    void interumReport(blknum_t bn) const {
        cerr << bn << sep;
        cerr << nBlocks << sep << nWithBlooms << sep << nWithoutBlooms << sep;
        cerr << nBlooms << sep << nBits << sep << nBytes << "\r";
        cerr.flush();
    }
};

//-----------------------------------------------------------------------------
extern bool handle_visit(blknum_t bn, void *data);
extern bool handle_stats(blknum_t bn, void *data);
extern bool handle_check(blknum_t bn, void *data);
extern bool handle_addrs(blknum_t bn, void *data);
extern bool handle_raw  (blknum_t bn, void *data);

//-----------------------------------------------------------------------------
extern bool accumAddrs (const CAddressAppearance& item, void *data);
extern bool transFilter(const CTransaction *trans, void *data);
