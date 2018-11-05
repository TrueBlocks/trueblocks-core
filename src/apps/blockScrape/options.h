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
#include "etherlib.h"

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    blknum_t  start;
    blknum_t  end;
    blknum_t  maxBlocks;
    bool      writeBlocks;
    string_q  exclusions;
    bool      keepAddrIdx;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);
    bool updateIndex(blknum_t bn);
    bool isExcluded(const address_t& addr) { return contains(exclusions, addr); }
};

//-------------------------------------------------------------------------
class CScraperCtx {
public:
    COptions        *opts;
    CBlock          *pBlock;
    CTransaction    *pTrans;
    CBloomArray     blooms;
    uint64_t         traceCount;
    uint64_t         maxTraceDepth;
    bool             reported;
    uint64_t         nAccounts;
    uint64_t         totAccounts;
    string_q         potList; // potential addresses from 'input' or log 'data'

    CScraperCtx(COptions *o) :
        opts(o),
        pBlock(NULL), pTrans(NULL),
        traceCount(0), maxTraceDepth(0), reported(false), nAccounts(0), totAccounts(0)
        { blooms.resize(1); blooms.at(0) = 0; }
    void addToBloom(const address_t& addr);
    bool scrape(CBlock& block);
};

//-----------------------------------------------------------------------------
extern bool visitNonEmptyBlock(CBlock& node, void *data);
extern bool visitEmptyBlock(CBlock& node, void *data);

//-------------------------------------------------------------------------
extern bool establishFullBlockIndex(void);
extern bool freshenLocalCache(COptions& options);
