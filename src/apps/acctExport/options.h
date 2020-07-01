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
#include "monitorcount.h"

// BEG_ERROR_DEFINES
// END_ERROR_DEFINES

#define CACHE_NONE (0)
#define CACHE_TXS (1 << 2)
#define CACHE_TRACES (1 << 3)
#define CACHE_BYCONFIG (1 << 4)
#define CACHE_BYUSER (1 << 5)
#define CACHE_BYDEFAULT (1 << 6)

using blk_addr_map_t = map<uint32_t, address_t>;
using addr_count_map_t = map<address_t, uint64_t>;
//-----------------------------------------------------------------------
class COptions : public COptionsBase {
  public:
    // BEG_CODE_DECLARE
    bool appearances;
    bool receipts;
    bool logs;
    bool traces;
    bool balances;
    bool accounting;
    bool articulate;
    bool skip_ddos;
    uint64_t max_traces;
    bool freshen;
    blknum_t freshen_max;
    bool deltas;
    bool emitter;
    bool count;
    blknum_t first_record;
    blknum_t max_records;
    // END_CODE_DECLARE

    int write_opt;  // cache options as resolved (see options.cpp for notes)

    CAbi abis;
    CMonitorArray monitors;
    CMonitorCountArray counts;
    CAppearanceArray_base items;
    uint64_t nExported;
    string_q className;
    address_t hackAppAddr;
    blk_addr_map_t prefundAddrMap;
    blk_addr_map_t blkRewardMap;

    addr_count_map_t toAddrMap;
    addr_count_map_t fromAddrMap;
    addr_count_map_t emitterAddrMap;
    addr_count_map_t toTraceAddrMap;
    addr_count_map_t fromTraceAddrMap;
    addr_count_map_t abiMap;

    uint32_t* ts_array;
    size_t ts_cnt;
    blknum_t latestBlock;
    time_q oldestMonitor;

    address_t accountedFor;
    string_q bytesOnly;

    uint64_t nProcessing;
    uint64_t nTransactions;
    uint64_t nCacheItemsRead;
    uint64_t nCacheItemsWritten;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);

    bool loadOneAddress(CAppearanceArray_base& apps, const address_t& addr);
    bool loadAllAppearances(void);

    bool exportAccounting(void);
    bool exportAppearances(void);
    bool exportCounts(void);
    bool exportReceipts(void);
    bool exportTraces(void);

    void addNeighbor(addr_count_map_t& map, const address_t& addr);
    bool reportNeighbors(void);
};

//--------------------------------------------------------------------------------
inline string_q plural(const string_q& in) {
    return substitute(toLower(in).substr(1, 1000) + "s", "logentrys", "logs");
}
