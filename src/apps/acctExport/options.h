#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
/*
 * Parts of this file were generated with makeClass --options. Edit only those parts of
 * the code outside of the BEG_CODE/END_CODE sections
 */
#include "pinlib.h"
#include "scrapestatistics.h"
#include "traversers.h"

// BEG_ERROR_DEFINES
// END_ERROR_DEFINES

typedef map<CAppearance_mon, CAddressArray> appAddrMap;

//-----------------------------------------------------------------------
class COptions : public CAbiOptions {
  public:
    // BEG_CODE_DECLARE
    CFourbyteArray fourbytes;
    bool appearances;
    bool receipts;
    bool logs;
    bool traces;
    bool statements;
    bool neighbors;
    bool accounting;
    bool articulate;
    bool cache;
    bool cache_traces;
    bool factory;
    bool count;
    blknum_t first_record;
    blknum_t max_records;
    bool relevant;
    bool clean;
    bool staging;
    bool unripe;
    string_q load;
    bool reversed;
    bool by_date;
    bool deep;
    bool skip_ddos;
    uint64_t max_traces;
    // END_CODE_DECLARE

    CAppearanceArray_mon monApps;
    CMonitorArray allMonitors;
    CMonitorArray possibles;
    const CMonitor* curMonitor;
    CAccountName accountedFor;

    CReconciliationMap prevStatements;

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

    CLogFilter logFilter;
    CAddressBoolMap assetFilter;

    CScrapeStatistics stats;

    blkrange_t fileRange;
    blkrange_t listRange;
    blkrange_t exportRange;

    string_q className;
    CMetaData meta;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);

    bool setDisplayFormatting(void);
    bool loadAllAppearances(void);

    bool handle_traversers(void);

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
    bool process_reconciliation(CTraverser* trav);
    bool isReconciled(CTraverser* trav) const;
    void cacheIfReconciled(CTraverser* trav, bool isNew) const;
    bool token_list_from_logs(CAccountNameMap& tokenList, const CTraverser* trav);

    size_t reportFreq{7};
    size_t reportDef{7};
    size_t slowQueries{0};
    size_t maxSlowQueries{50};

    // Used as temporary data to count neighbor traversals
    size_t neighborCount{0};
    CIndexArchive* theIndex{nullptr};
    bool showAddrsInTx(CTraverser* trav, const blkrange_t& range, const CAppearance_mon& app);
};

//--------------------------------------------------------------------------------
extern bool visitOnLoad(CAppearance_mon& app, void* data);
extern bool visitFinalIndexFiles(const string_q& path, void* data);
extern bool visitStagingIndexFiles(const string_q& path, void* data);
extern bool visitUnripeIndexFiles(const string_q& path, void* data);
extern bool isTokenFunc(const string_q& input);
extern bool isTokenTopic(const CLogEntry* log);
extern bool fourByteFilter(const string_q& input, const COptions* opt);
