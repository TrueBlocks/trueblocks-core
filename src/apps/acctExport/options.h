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
#include "acctlib.h"
#include "scrapestatistics.h"
#include "traversers.h"

// BEG_ERROR_DEFINES
// END_ERROR_DEFINES

typedef struct CReverseAppMapEntry {
  public:
    uint32_t n;
    uint32_t blk;
    uint32_t tx;
} CReverseAppMapEntry;

//---------------------------------------------------------------------------
class CIndexArchiveWithNeighborMaps : public CIndexArchive {
  public:
    CBlockRangeArray reverseAddrRanges;
    CReverseAppMapEntry* reverseAppMap{nullptr};
    explicit CIndexArchiveWithNeighborMaps(bool mode) : CIndexArchive(mode) {
        reverseAppMap = nullptr;
    }
    ~CIndexArchiveWithNeighborMaps();
    bool LoadReverseMaps(const blkrange_t& range);

  private:
    void clean(void) {
        if (reverseAppMap) {
            delete[] reverseAppMap;
            reverseAppMap = nullptr;
        }
        reverseAddrRanges.clear();
    }
};

//-----------------------------------------------------------------------
class COptions : public CStatementOptions {
  public:
    // BEG_CODE_DECLARE
    CFourbyteArray fourbytes;
    bool appearances;
    bool receipts;
    bool logs;
    bool traces;
    bool neighbors;
    bool accounting;
    bool statements;
    bool articulate;
    bool cache;
    bool cache_traces;
    bool count;
    uint64_t first_record;
    uint64_t max_records;
    bool relevant;
    string_q flow;
    bool factory;
    string_q load;
    bool reversed;
    // END_CODE_DECLARE

    bool skip_ddos{true};
    uint64_t max_traces{250};
    CMonitorArray allMonitors;
    const CMonitor* curMonitor;

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

    CScrapeStatistics stats;

    blkrange_t fileRange;
    blkrange_t exportRange;

    string_q className;
    CMetaData meta;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);

    bool setDisplayFormatting(void);
    bool loadMonitors(void);

    bool handle_traversers(void);

    void addNeighbor(CAddressUintMap& map, const address_t& addr);
    void markNeighbors(const CTransaction& trans);
    bool articulateAll(CTransaction& trans);
    bool reportNeighbors(void);

    bool isEmitter(const address_t& test) const;
    bool wasEmittedBy(const address_t& test) const;
    bool isRelevant(const CLog& log) const;
    bool fourByteFilter(const string_q& input) const;

    void writePerformanceData(void);

    // Used as temporary data to count neighbor traversals
    size_t neighborCount{0};
    CIndexArchiveWithNeighborMaps* theIndex{nullptr};
    bool showAddrsInTx(CTraverser* trav, const blkrange_t& range, const CAppearance_mon& app);
};

//--------------------------------------------------------------------------------
extern bool visitOnLoad(CAppearance_mon& app, void* data);
