#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "acctlib.h"
#include "displayapp.h"

using uint_addr_mp = map<uint32_t,address_t>;

//-----------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    CAbi abis;
    CAccountWatchArray monitors;
    CAccountWatchArray named;
    CAppearanceArray_base items;
    bool writeTxs;
    bool writeTraces;
    bool skipDdos;
    size_t maxTraces;
    bool articulate;
    size_t nExported;
    bool doAppearances;
    bool doLogs;
    bool doTraces;
    bool doBalances;
    bool doABIs;
    bool freshen_only;
    bool count_only;
    bool deltas_only;
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
    string_q postProcess(const string_q& which, const string_q& str) const;

    bool loadOneAddress(CAppearanceArray_base& apps, const address_t& addr);
    bool loadAllAppearances(void);
    bool exportData(void);
    bool exportBalances(void);
};
