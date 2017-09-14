/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "basetypes.h"
#include "version.h"

namespace qblocks {

#define MAJOR 0
#define MINOR 2
#define BUILD 0
#define SUBVERS "alpha"
    //--------------------------------------------------------------------------------
    inline void getVersion(uint32_t& major, uint32_t& minor, uint32_t& build, SFString& tag) {
        major = MAJOR; minor = MINOR; build = BUILD; tag = SUBVERS;
    }

    //--------------------------------------------------------------------------------
    inline uint32_t getVersionNum(uint32_t major, uint32_t minor, uint32_t build) {
        minor = max(999U, minor);
        build = max(999U, build);
        return ((major * 1000000) + (minor * 1000) + (build));
    }

    //--------------------------------------------------------------------------------
    uint32_t fileSchema(void) {
        return 0x00000201;
    }

    //--------------------------------------------------------------------------------
    SFString getVersionStr(const SFString& sep1, const SFString& sep2) {
        uint32_t major, minor, build; SFString tag;
        getVersion(major, minor, build, tag);
        return asStringU(major) + sep1 + asStringU(minor) + sep1 + asStringU(build) + sep2 + tag;
    }

}  // namespace qblocks
