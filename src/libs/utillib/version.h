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
    extern uint32_t getVersionNum(uint16_t maj, uint16_t min, uint16_t build);
    extern uint32_t getVersionNum(void);

}  // namespace qblocks
