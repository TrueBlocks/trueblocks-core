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

#define SFAddress      SFString
#define SFHash         SFString
#define SFBloom        SFUintBN
#define blknum_t       uint64_t
#define txnum_t        uint32_t
#define lognum_t       uint32_t

#define toHash(a)      (a)
#define toTopic(a)     canonicalWei(a)
#define toBloom(a)     canonicalWei(a)
#define toWei(a)       canonicalWei(a)
#define toUnsigned(a)  (SFUint32)((a).startsWith("0x")?hex2Long((a)):toLongU((a)))

#define fromAddress(a)  ((a).empty() ? "0x0" : (a))
#define fromHash(a)     ((a).empty() ? "0x0" : (a))
#define fromWei(a)      to_string((a)).c_str()
#define fromTopic(a)    ("0x"+padLeft(toLower(SFString(to_hex((a)).c_str())),64,'0'))
#define fromBloom(a)    ((a)==0?"0x0":"0x"+padLeft(toLower(SFString(to_hex((a)).c_str())),512,'0'))
#define fromUnsigned(a) asStringU((a))

//------------------------------------------------------
inline SFAddress toAddress(const SFString& strIn)
{
    // Strip it if it's there. We will put it back
    SFString ret = strIn.Substitute("0x","");

    // Shorten, but only if all leading zeros
    if (ret.length()==64 && ret.startsWith("000000000000000000000000"))
        ret.Replace("000000000000000000000000","");

    // Special case
    if (ret.empty())
        ret = "0";

    return "0x" + ret;
}
}
