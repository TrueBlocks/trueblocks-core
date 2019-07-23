#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "acctlib.h"

//-----------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    CAccountWatchArray monitors;
    CAccountWatchArray named;
    CAppearanceArray_base items;
    bool writeBlocks;
    bool writeTrxs;
    bool writeTrcs;
    bool skipDdos;
    size_t maxTraces;
    bool articulate;
    bool doLogs;
    bool doTraces;
    uint32_t *ts_array;
    size_t ts_cnt;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);
    string_q postProcess(const string_q& which, const string_q& str) const;

    map<uint32_t,address_t> prefundMap;
    map<uint32_t,address_t> blkRewardMap;
    bool loadMonitorData(CAppearanceArray_base& apps, const address_t& addr);
    bool loadData(void);
    bool exportData(void);
    bool loadTraces(CTransaction& trans, const CAppearance_base *item);
    bool loadTsArray(blknum_t blk);
};

//-----------------------------------------------------------------------
extern bool isInTrace(CTrace& trace, const address_t& addr);
extern bool excludeTrace(const CTransaction *trans, size_t maxTraces);
extern bool freshenTsDatabase(blknum_t blk, CUintArray& tsArray);  // array may be empty in which case we query the node
