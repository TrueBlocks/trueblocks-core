/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "tokenbal.h"

namespace qblocks {

//-------------------------------------------------------------------------
SFUintBN getTokenBalance(const SFAddress& token, const SFAddress& holder, blknum_t blockNum) {

    ASSERT(isAddress(token));
    ASSERT(isAddress(holder));

    SFString t = "0x" + padLeft(token.substr(2), 40, '0');  // address to send the command to
    SFString h =        padLeft(holder.substr(2), 64, '0'); // encoded data for the transaction

    SFString cmd = "[{\"to\": \"[TOKEN]\", \"data\": \"0x70a08231[HOLDER]\"}, \"[BLOCK]\"]";
    cmd.Replace("[TOKEN]",  "0x"+t);
    cmd.Replace("[HOLDER]", h);
    cmd.Replace("[BLOCK]",  asStringU(blockNum));

    return toWei(callRPC("eth_call", cmd, false));
}

}  // namespace qblocks
