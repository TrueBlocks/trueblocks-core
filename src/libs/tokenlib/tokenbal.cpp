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

    SFString t = token.substr(2);
    t = padLeft(t, 40, '0');

    SFString h = holder.substr(2);
    h = padLeft(h, 64, '0');

    SFString cmd = "[{\"to\": \"[TOKEN]\", \"data\": \"0x70a08231[HOLDER]\"}, \"[BLOCK]\"]";
    cmd.Replace("[TOKEN]",  "0x"+t);
    cmd.Replace("[HOLDER]", h);
    cmd.Replace("[BLOCK]",  asStringU(blockNum));

    extern SFString callRPC(const SFString& method, const SFString& params, bool raw);
    SFString ret = callRPC("eth_call", cmd, false);
    return toWei(ret);
}

}  // namespace qblocks
