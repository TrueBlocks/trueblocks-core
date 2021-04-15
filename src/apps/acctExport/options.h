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
#include "acctscrapestats.h"
#include "traverser.h"

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

    CAppearanceArray_base apps;
    CMonitorArray allMonitors;
    CMonitorArray possibles;
    address_t accountedFor;
    string_q accountedForName;

    CReconciliation lastStatement;
    CReconciliationMap prevStatements;
    CMonitorCountArray counts;
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

    CAcctScrapeStats stats;

    blkrange_t fileRange;
    blkrange_t listRange;
    size_t visitTypes;

    string_q className;
    CBlockProgress bp;
    bool firstOut;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);

    bool setDisplayFormatting(void);
    bool loadOneAddress(const CMonitor& monitor, CAppearanceArray_base& arrayOut);
    bool loadAllAppearances(void);

    bool handle_accounting(void);
    bool handle_appearances(void);
    bool handle_logs(void);
    bool handle_receipts(void);
    bool handle_traces(void);
    bool handle_counts(void);
    bool process_clean(void);
    bool process_rm(const CAddressArray& addrs);
    bool process_freshen(void);

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
