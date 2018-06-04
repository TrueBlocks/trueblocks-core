#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "etherlib.h"
#include "node.h"

namespace qblocks {

    //-------------------------------------------------------------------------
    // Helps debug a bloom filter
#define dbgBloom(a) bloom2Bytes(a).Substitute("0"," ")

    //-------------------------------------------------------------------------
    inline SFUintBN makeBloom(const SFString& hexIn) {
        if (hexIn.empty() || !hexIn.startsWith("0x"))
            return 0;

extern SFString getSha3 (const SFString& hexIn);
        SFString sha = getSha3(hexIn);
        SFUintBN bloom;
        for (uint32_t i=0;i<3;i++)
            bloom |= (SFUintBN(1) << (strtoul((const char*)"0x"+sha.substr(2+(i*4),4),NULL,16))%2048);
        return bloom;
    }

    //-------------------------------------------------------------------------
    inline SFBloom joinBloom(const SFBloom& b1, const SFBloom& b2) {
        return (b1 | b2);
    }

    //-------------------------------------------------------------------------
    inline bool isBloomHit(const SFUintBN& test, const SFUintBN filter) {
        return ((test & filter) == test);
    }

    //-------------------------------------------------------------------------
    inline bool isBloomHit(const SFString& hexIn, const SFUintBN filter) {
        return isBloomHit(makeBloom(hexIn),filter);
    }

    //----------------------------------------------------------------------------------
    extern bool compareBlooms(const SFBloom& b1, const SFBloom& b2, SFString& str);
    extern SFString formatBloom(const SFBloom& b1, bool bits);
    extern bool addAddrToBloom(const SFAddress& addr, SFBloomArray& blooms, uint32_t maxBits);

    //----------------------------------------------------------------------------------
    extern bool readBloomArray (      SFBloomArray& blooms, const SFString& fileName);
    extern bool writeBloomArray(const SFBloomArray& blooms, const SFString& fileName);
    extern SFString reportBloom(const SFBloomArray& blooms);

}  // namespace qblocks

