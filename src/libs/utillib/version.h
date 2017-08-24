#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

#include "version.h"

namespace qblocks {

    //--------------------------------------------------------------------------------
#define MAJOR 0
#define MINOR 2
#define BUILD 0
#define SUBVERS "alpha"
    //--------------------------------------------------------------------------------
    inline void getVersion(uint32_t& major, uint32_t& minor, uint32_t& build, SFString& tag) {
        major = MAJOR; minor = MINOR; build = BUILD;
        tag = SUBVERS;
    }

    //--------------------------------------------------------------------------------
    inline SFString getVersionStr(const SFString& sep=".") {
        uint32_t major, minor, build;
        SFString tag;
        getVersion(major, minor, build, tag);
        return asStringU(major) + sep + asStringU(minor) + sep + asStringU(build) + sep + tag;
    }

    //--------------------------------------------------------------------------------
    inline uint32_t currentSchema(void) {
        return ((MAJOR * 1000000) + (MINOR * 1000) + (BUILD));
    }

}  // namespace qblocks
