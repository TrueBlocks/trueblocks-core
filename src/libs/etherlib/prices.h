#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "utillib.h"

namespace qblocks {

extern double getPriceInUsd(blknum_t bn, const address_t& address = "");
extern double getPriceFromMaker_EthUsd(blknum_t bn);
extern double getPriceFromUni_EthUsd(blknum_t bn);
extern string_q addr_2_Pad64(const address_t& addr);

}  // namespace qblocks
