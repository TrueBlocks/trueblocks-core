#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
/*
 * Parts of this file were generated with makeClass. Edit only those parts of the code
 * outside of the BEG_CODE/END_CODE sections
 */
#include "etherlib.h"
#include "acctlib.h"
#include "displayapp.h"
#include "counts.h"
#include "ipfshash.h"

// BEG_ERROR_DEFINES
// END_ERROR_DEFINES

#define CACHE_NONE (0)
#define CACHE_TXS (1 << 2)
#define CACHE_TRACES (1 << 3)
#define CACHE_BYCONFIG (1 << 4)
#define CACHE_BYUSER (1 << 5)
#define CACHE_BYDEFAULT (1 << 6)

using uint_addr_mp = map<uint32_t, address_t>;
//-----------------------------------------------------------------------
class COptions : public COptionsBase {
  public:
    // BEG_CODE_DECLARE
    bool appearances;
    bool receipts;
    bool logs;
    bool traces;
    bool balances;
    bool hashes_only;
    bool count_only;
    bool articulate;
    bool skip_ddos;
    uint64_t max_traces;
    bool grab_abis;
    bool freshen;
    bool deltas;
    blknum_t occurrence;
    bool emitter;
    // END_CODE_DECLARE

    int write_opt;  // cache options as resolved (see options.cpp for notes)

    CAbi abis;
    CAccountWatchArray monitors;
    CAccountWatchArray named;
    CAppearanceArray_base items;
    size_t nExported;
    string_q className;
    address_t hackAppAddr;
    uint_addr_mp prefundAddrMap;
    uint_addr_mp blkRewardMap;
    map<address_t, bool> abiMap;
    uint32_t* ts_array;
    size_t ts_cnt;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);

    bool loadOneAddress(CAppearanceArray_base& apps, const address_t& addr);
    bool loadAllAppearances(void);
    bool exportData(void);
    bool exportCounts(void);
    bool exportBalances(void);
    bool exportIPFSHashes(void);
};
