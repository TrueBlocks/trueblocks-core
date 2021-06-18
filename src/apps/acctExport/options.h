#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
/*
 * Parts of this file were generated with makeClass --options. Edit only those parts of
 * the code outside of the BEG_CODE/END_CODE sections
 */
#include "pinlib.h"
#include "scrapestatistics.h"
#include "traversers.h"

// BEG_ERROR_DEFINES
// END_ERROR_DEFINES

//-----------------------------------------------------------------------
class COptions : public CAbiOptions {
  public:
    // BEG_CODE_DECLARE
    CTopicArray topics;
    CFourbyteArray fourbytes;
    bool appearances;
    bool receipts;
    bool logs;
    bool traces;
    bool accounting;
    bool tokens;
    bool articulate;
    bool cache_txs;
    bool cache_traces;
    bool skip_ddos;
    uint64_t max_traces;
    bool factory;
    bool emitter;
    CAddressArray source;
    bool relevant;
    bool count;
    blknum_t first_record;
    blknum_t max_records;
    bool clean;
    bool staging;
    bool unripe;
    string_q load;
    // END_CODE_DECLARE

    CAppearanceArray_base2 apps;
    CAppearanceArray_base2 tmp;
    CMonitorArray allMonitors;
    CMonitorArray possibles;
    address_t accountedFor;
    string_q accountedForName;
    blkrange_t blockRange;

    CReconciliation lastStatement;
    CReconciliationMap prevStatements;
    CPinnedChunkArray pinList;

    CBlockAddressMap prefundAddrMap;
    CBlockAddressMap blkRewardMap;

    // neighbor maps
    CAddressUintMap toAddrMap;
    CAddressUintMap fromAddrMap;
    CAddressUintMap emitterAddrMap;
    CAddressUintMap creationMap;
    CAddressUintMap toTraceAddrMap;
    CAddressUintMap fromTraceAddrMap;

    // abiMap allows fast access to abis
    CAddressUintMap abiMap;

    CScrapeStatistics stats;

    blkrange_t fileRange;
    blkrange_t listRange;

    string_q className;
    CBlockProgress bp;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);

    bool setDisplayFormatting(void);
    bool loadOneAddress(const CMonitor& monitor, CAppearanceArray_base2& arrayOut);
    bool loadAllAppearances(void);

    bool process_clean(void);
    bool process_rm(const CAddressArray& addrs);
    bool process_freshen(void);

    bool visitBinaryFile(const string_q& path, void* data);
    void addNeighbor(CAddressUintMap& map, const address_t& addr);
    void markNeighbors(const CTransaction& trans);
    bool articulateAll(CTransaction& trans);
    bool reportNeighbors(void);

    bool establishIndexChunk(const string_q& fileName);
    bool isEmitter(const address_t& test) const;
    bool wasEmittedBy(const address_t& test) const;
    bool isRelevant(const CLogEntry& log) const;

    void writePerformanceData(void);

    bool queryFlatFile(const string_q& path, bool sorted);
};

//--------------------------------------------------------------------------------
extern bool visitFinalIndexFiles(const string_q& path, void* data);
extern bool visitStagingIndexFiles(const string_q& path, void* data);
extern bool visitUnripeIndexFiles(const string_q& path, void* data);
extern bool isTokenFunc(const string_q& input);
extern bool isTokenTopic(const topic_t& topic);
extern bool fourByteFilter(const string_q& input, const COptions* opt);
//--------------------------------------------------------------------------------
inline string_q plural(const string_q& in) {
    return substitute(toLower(in).substr(1, 1000) + "s", "logentrys", "logs");
}

//-----------------------------------------------------------------------
extern bool process_reconciliation(COptions* options, CTransaction& trans, CReconciliationMap& prev, blknum_t next,
                                   bool tokens);
