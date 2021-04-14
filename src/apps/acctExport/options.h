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
#include "pinlib.h"
#include "traverser.h"

// BEG_ERROR_DEFINES
// END_ERROR_DEFINES

//-----------------------------------------------------------------------
class COptions : public CAbiOptions {
  public:
    // BEG_CODE_DECLARE
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
    bool freshen;
    bool factory;
    bool emitter;
    CAddressArray emitted_by;
    bool relevant;
    bool count;
    blknum_t first_record;
    blknum_t max_records;
    bool clean;
    // END_CODE_DECLARE

    CAcctScrapeStats stats;
    CMonitorArray allMonitors;
    CMonitorArray possibles;
    blkrange_t fileRange;
    size_t visitTypes;
    CBlockProgress bp;
    bool firstOut;

    CMonitorCountArray counts;
    CAppearanceArray_base apps;
    string_q className;
    address_t hackAppAddr;
    string_q hackAppName;
    CBlockAddressMap prefundAddrMap;
    CBlockAddressMap blkRewardMap;
    CPinnedChunkArray pinList;

    CAddressUintMap toAddrMap;
    CAddressUintMap fromAddrMap;
    CAddressUintMap emitterAddrMap;
    CAddressUintMap creationMap;
    CAddressUintMap toTraceAddrMap;
    CAddressUintMap fromTraceAddrMap;
    CAddressUintMap abiMap;

    address_t accountedFor;
    blknum_t latestBlock;
    time_q oldestMonitor;

    string_q bytesOnly;

    uint64_t nProcessing;
    uint64_t nTransactions;
    uint64_t nCacheItemsRead;
    uint64_t nCacheItemsWritten;

    blkrange_t listRange;
    CReconciliation lastStatement;
    CReconciliationMap prevStatements;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);

    bool setDisplayFormatting(void);
    bool loadOneAddress(const CMonitor& monitor, CAppearanceArray_base& arrayOut);
    bool loadAllAppearances(void);
    bool freshen_internal(void);

    bool handle_accounting(void);
    bool handle_appearances(void);
    bool handle_logs(void);
    bool handle_receipts(void);
    bool handle_traces(void);
    bool handle_counts(void);
    bool handle_clean(void);
    bool handle_rm(const CAddressArray& addrs);

    bool visitBinaryFile(const string_q& path, void* data);
    void addNeighbor(CAddressUintMap& map, const address_t& addr);
    void markNeighbors(const CTransaction& trans);
    void articulateAll(CTransaction& trans);
    bool reportNeighbors(void);

    bool establishIndexChunk(const string_q& fileName);
    bool isEmitter(const address_t& test) const;
    bool wasEmittedBy(const address_t& test) const;
    bool isRelevant(const CLogEntry& log) const;
};

//--------------------------------------------------------------------------------
extern bool visitFinalIndexFiles(const string_q& path, void* data);
extern bool visitStagingIndexFiles(const string_q& path, void* data);
extern bool visitUnripeIndexFiles(const string_q& path, void* data);
extern bool isTokenFunc(const string_q& input);
extern bool isTokenTopic(const topic_t& topic);
//--------------------------------------------------------------------------------
inline string_q plural(const string_q& in) {
    return substitute(toLower(in).substr(1, 1000) + "s", "logentrys", "logs");
}

//--------------------------------------------------------------------------------
inline bool isJson(void) {
    return (expContext().exportFmt == JSON1 || expContext().exportFmt == API1 || expContext().exportFmt == NONE1);
}

#define VIS_FINAL (1 << 1)
#define VIS_STAGING (1 << 2)
#define VIS_UNRIPE (1 << 3)

//-----------------------------------------------------------------------
extern bool process_reconciliation(COptions* options, CTransaction& trans, CReconciliationMap& prev, blknum_t next,
                                   bool tokens);
