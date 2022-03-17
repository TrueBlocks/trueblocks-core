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
#include "utillib.h"

struct CMetaData {
  public:
    blknum_t unripe;
    blknum_t ripe;
    blknum_t staging;
    blknum_t finalized;
    blknum_t client;
    uint64_t chainId;
    uint64_t networkId;
    string_q chain;
};

//-------------------------------------------------------------------------
extern CMetaData getMetaData(void);
extern blknum_t getLatestBlock_client(void);
