#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

#include "conversions.h"

namespace qblocks {

    //--------------------------------------------------------------------------------
    extern SFString getVersionStr(const SFString& sep1=".", const SFString& sep2="-");
    extern uint32_t dataSchema   (void);
    extern uint32_t fileSchema   (void);

}  // namespace qblocks
