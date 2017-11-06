/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "blockoptions.h"

//--------------------------------------------------------------------------------
CBlockOptions::CBlockOptions(void) {
}

//--------------------------------------------------------------------------------
SFString CBlockOptions::getBlockNumList(void) const {
    SFString ret;
    SFString list = blocks.toString();
    while (!list.empty()) {
        SFString val = nextTokenClear(list, '|');
        blknum_t bn = toLongU(val);
        if (isHash(val)) {
            CBlock block;
            if (!getBlock(block, val)) {
                cerr << "Block hash '" << val << "' does not appear to be a valid block hash. Quitting...";
                exit(0);
            }
            bn = block.blockNumber;
        }
        ret += asStringU(bn) + "|";
    }
    return ret;
}
