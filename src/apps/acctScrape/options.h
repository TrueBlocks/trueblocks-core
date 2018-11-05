#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "acctlib.h"

//-----------------------------------------------------------------------------
class CStats {
public:
    blknum_t nSeen;
    blknum_t nSkipped;
    blknum_t nHit;
    CStats(void) : nSeen(0), nSkipped(0), nHit(0) { }
};
#define nQueried nSkipped

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    blknum_t           lastBlock;
    blknum_t           minWatchBlock;
    blknum_t           maxWatchBlock;
    blknum_t           maxBlocks;
    blknum_t           oneBlock;
    blknum_t           oneTrans;
    timestamp_t        lastTimestamp;
    bool               ignoreAddrs;
    bool               ignoreBlooms;
    bool               writeBlocks;
    string_q           exclusions;
    blknum_t           firstBlock;
    blknum_t           nBlocks;
    CStats             blkStats;
    CStats             addrStats;
    CStats             transStats;
    CStats             traceStats;
    bool               blockCounted;
    CAccountWatchArray monitors;
    CArchive           txCache;
    string_q           name;
    uint64_t           debugging;
    uint64_t           logLevel;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);
    bool isExcluded(const address_t& addr) { return contains(exclusions, addr); }
    bool loadMonitors(const CToml& toml);
    friend ostream& operator<<(ostream& os, const COptions& item);
    string_q finalReport(double timing) const;
};

//-----------------------------------------------------------------------
extern bool visitBloomFilters(const string_q& path, void *data);
extern void myQuitHandler    (int s);
