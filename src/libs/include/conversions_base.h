#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

namespace qblocks {

    using timestamp_t = int64_t;
#define isHex(cc) (isdigit(cc) || (cc) == 'x' || ((cc) >= 'a' && (cc) <= 'f') || ((cc) >= 'A' && (cc) <= 'F'))
    inline bool isHexStr(const SFString& str) {
        if (!str.startsWith("0x"))
            return false;
        for (size_t i = 2 ; i < str.length() ; i++)
            if (!isHex(str[(int)i]))
                return false;
        return true;
    }
}  // namespace qblocks
