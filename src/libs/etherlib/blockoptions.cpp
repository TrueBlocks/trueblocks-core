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
#include "blockoptions.h"

static uint64_t findBlockNumByHash(const SFHash& hash, void *data);
//--------------------------------------------------------------------------------
CBlockOptions::CBlockOptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
void CBlockOptions::Init(void) {
    blocks.hashFind = findBlockNumByHash;
}

//--------------------------------------------------------------------------------
uint64_t findBlockNumByHash(const SFHash& hash, void *data) {
    ASSERT(isHash(hash));
    CBlock block;
    if (!getBlock(block, hash)) {
        cerr << "Block hash '" << hash << "' does not appear to be a valid block hash. Quitting...";
        exit(0);
    }
    return block.blockNumber;
}

//--------------------------------------------------------------------------------
bool blockNumToString(uint64_t num, void *data) {
    if (num != NOPOS) {
        SFString *str = (SFString*)data;
        *str += (asStringU(num) + "|");
    }
    return true;
}

//--------------------------------------------------------------------------------
SFString CBlockOptions::getBlockNumList(void) {
    SFString ret;
    blocks.hashFind = findBlockNumByHash;
    blocks.forEveryBlockNumber(blockNumToString, &ret);
    return ret;
}
