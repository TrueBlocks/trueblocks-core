/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "blockoptions.h"

static uint64_t findBlockNumByHash(const SFHash& hash, void *data);
//--------------------------------------------------------------------------------
CBlockOptions::CBlockOptions(void) {
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
