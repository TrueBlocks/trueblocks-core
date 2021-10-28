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
#include "etherlib.h"

// BEG_ERROR_DEFINES
#define ERR_NOCACHEUNCLE 1
#define ERR_NOCACHEADDRESS 2
#define ERR_TRACINGREQUIRED 3
#define ERR_NOTRACEADDRESS 4
#define ERR_TRACEHASHEXCLUSIVE 5
#define ERR_ATLEASTONEBLOCK 6
#define ERR_EMTOPONLYWITHLOG 7
#define ERR_ARTWITHOUTLOGS 8
// END_ERROR_DEFINES

//-----------------------------------------------------------------------------
class COptions : public CBlockOptions {
  public:
    // BEG_CODE_DECLARE
    bool hashes;
    bool uncles;
    bool trace;
    bool logs;
    bool articulate;
    bool count;
    bool cache;
    blknum_t list_count;
    // END_CODE_DECLARE

    uint64_t nProcessed{0};
    CBlock latestBlock;
    uint64_t addrCounter;
    string_q filterType;
    timestamp_t secsFinal;
    blknum_t listOffset;
    CLogFilter logFilter;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command) override;
    void Init(void) override;

    bool isMulti(void) const;
    bool handle_appearances(blknum_t num, void* data);
    bool handle_block_summaries(blknum_t start, blknum_t count);
    bool handle_logs(void);
    bool processFastPath(void);
};

//-----------------------------------------------------------------------------
extern bool visitBlock(uint64_t num, void* data);
extern bool traceBlock(uint64_t num, void* data);
extern void interumReport(ostream& os, blknum_t i);

//----------------------------------------------------------------
extern void oneAppearance(const CAppearance& item, void* data);
extern bool visitAddrs(const CAppearance& item, void* data);
extern bool transFilter(const CTransaction* trans, void* data);
