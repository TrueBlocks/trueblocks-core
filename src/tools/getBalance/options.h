#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
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
class CState {
public:
    bool needsNewline;
    blknum_t latestBlock;
    blknum_t earliestBlock;
    SFUintBN totalVal;
    SFUintBN lastBal;
    SFAddress curAddr;
    CState(void) { needsNewline = true; latestBlock = getLatestBlockFromClient(); totalVal = 0; lastBal = 0; }
};

//-----------------------------------------------------------------------------
class COptions : public CBlockOptions {
public:
    CState state;
    SFAddressArray addrs;
    bool asData;
    bool noZero;
    bool total;
    bool changes;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command) override;
    void Init(void) override;
    string_q postProcess(const string_q& which, const string_q& str) const override;
};
