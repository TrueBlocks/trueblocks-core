#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "utillib.h"

namespace qblocks {

extern double getPriceInUsd(blknum_t bn, string_q& priceSource, const address_t& address = "");

}  // namespace qblocks
