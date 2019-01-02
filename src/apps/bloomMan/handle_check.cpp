/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//-------------------------------------------------------------------------
bool handle_check(blknum_t bn, void *data) {

    CBlock block;
    getBlock(block, bn);

    CAddressArray array;
    block.forEveryUniqueAddress(accumAddrs, transFilter, &array);

    COptions *options = (COptions *)data;
    options->address_list = "";
    for (auto addr : array)
        options->address_list += (addr + "|");

    return handle_addrs(bn, data);
}
