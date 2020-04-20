#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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
#define dbgBloom(a) substitute(bloom_2_Bytes(a), "0", " ")

//-------------------------------------------------------------------------
inline size_t bitsTwiddled(bloom_t n) {
    size_t count = 0;
    while (n != 0) {
        biguint_t x = n - 1;
        biguint_t y = n & x;
        n = y;
        count++;
    }
    return count;
}

//-------------------------------------------------------------------------
inline bloom_t makeBloom(const string_q& hexIn) {
    if (hexIn.empty() || !startsWith(hexIn, "0x"))
        return 0;

    string_q sha = keccak256(hexIn);
    bloom_t bloom;
    for (size_t i = 0; i < 3; i++)
        bloom |= (bloom_t(1) << (strtoul(("0x" + extract(sha, 2 + (i * 4), 4)).c_str(), NULL, 16)) % 2048);
    return bloom;
}

//-------------------------------------------------------------------------
inline bloom_t joinBloom(const bloom_t& b1, const bloom_t& b2) {
    return (b1 | b2);
}

//-------------------------------------------------------------------------
inline bool isBloomHit(const bloom_t& test, const bloom_t filter) {
    return ((test & filter) == test);
}

//-------------------------------------------------------------------------
inline bool isBloomHit(const string_q& hexIn, const bloom_t filter) {
    return isBloomHit(makeBloom(hexIn), filter);
}

//----------------------------------------------------------------------------------
extern bool addAddrToBloom(const address_t& addr, CBloomArray& blooms, size_t maxBits);

//----------------------------------------------------------------------------------
extern bool readBloomFromBinary(CBloomArray& blooms, const string_q& fileName);
extern bool writeBloomArray(const CBloomArray& blooms, const string_q& fileName);
extern string_q reportBloom(const CBloomArray& blooms);

//--------------------------------------------------------------------------------
extern string_q bloom_2_BitStr(const bloom_t& bl);
extern string_q bloom_2_ByteStr(const bloom_t& bl);
extern string_q compareBlooms(const bloom_t& b1, const bloom_t& b2);

}  // namespace qblocks
