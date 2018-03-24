#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

#include "utillib.h"

namespace qblocks {

#define byzantiumBlock     4370000

//-----------------------------------------------------------------------
inline bool ddosRange(blknum_t bn) {
    return (bn >= 2283440 && bn <= 2718436);
}

} // namespace qblocks

