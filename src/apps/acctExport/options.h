#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "acctlib.h"
#include "displayapp.h"

using uint_addr_mp = map<uint32_t,address_t>;
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
    bool write_blocks;
    bool write_txs;
    bool write_traces;
    bool skip_ddos;
    uint64_t max_traces;
    bool grab_abis;
    bool freshen;
    bool deltas;
    // END_CODE_DECLARE

    CAbi abis;
    CAccountWatchArray monitors;
    CAccountWatchArray named;
    CAppearanceArray_base items;
    size_t nExported;
    string_q className;
    address_t hackAppAddr;
    uint_addr_mp prefundAddrMap;
    uint_addr_mp blkRewardMap;
    map<address_t,bool>     abiMap;
    uint32_t *ts_array;
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
};
