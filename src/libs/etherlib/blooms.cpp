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
/*
 * This file was generated with makeClass. Edit only those parts of the code inside
 * of 'EXISTING_CODE' tags.
 */
#include "blooms.h"

namespace qblocks {

    //-------------------------------------------------------------------------
    bool compareBlooms(const SFBloom& b1, const SFBloom& b2, string_q& str) {
        if (verbose > 2) {
            str = "\n\tbits1: " + uint_2_Str(bitsTwiddled(b1)) + " bits2: " + uint_2_Str(bitsTwiddled(b2));
            string_q s1 = substitute(bloom_2_Bits(b1), "0", ".");
            string_q s2 = substitute(bloom_2_Bits(b2), "0", ".");
            for (size_t i = 0 ; i < 16 ; i++) {
                string_q m1, m2;
                for (size_t j = 0 ; j < 128 ; j = j + 10) {
                    m1 += extract(extract(s1, i*128, 128), j, 10) + " ";
                    m2 += extract(extract(s2, i*128, 128), j, 10) + " ";
                }
                str += ("\n\t" + cRed + m1 + cOff + "\n\t" + m2);
            }
        } else if (verbose > 1) {
            str = "\n\tbits: " + uint_2_Str(bitsTwiddled(b1)) + " " + uint_2_Str(bitsTwiddled(b2));
            string_q s1 = substitute(substitute(bloom_2_Bytes(b1), "0x", ""), "0", ".");
            string_q s2 = substitute(substitute(bloom_2_Bytes(b2), "0x", ""), "0", ".");
            for (size_t i = 0 ; i < 4 ; i++) {
                string_q m1, m2;
                for (size_t j = 0 ; j < 128 ; j = j + 10) {
                    m1 += extract(extract(s1, i*128, 128), j, 10) + " ";
                    m2 += extract(extract(s2, i*128, 128), j, 10) + " ";
                }
                str += ("\n\t" + cRed + m1 + cOff + "\n\t" + m2 + "\n");
            }
        }
        return isBloomHit(b1, b2);
    }

    //----------------------------------------------------------------------------------
    bool addAddrToBloom(const address_t& addr, SFBloomArray& blooms, size_t maxBits) {
        // Initialize if not already
        if (blooms.size() == 0)
            blooms.push_back(0);
        size_t cnt = blooms.size();
        blooms.at(cnt - 1) = joinBloom(blooms[cnt - 1], makeBloom(addr));  // requires the non-const reference
        if (bitsTwiddled(blooms[cnt - 1]) > maxBits) {
            blooms.push_back(0);  // start a new bloom
            return true;
        }
        return false;
    }

    //-----------------------------------------------------------------------
    string_q reportBloom(const SFBloomArray& blooms) {
        string_q ret;
        for (size_t i = 0; i < blooms.size(); i++) {
            uint64_t bits = bitsTwiddled(blooms[i]);
            if (bits) {
                ret += uint_2_Str(bits);
                if (i < blooms.size()-1)
                    ret += ",";
            }
        }
        if (ret.empty())
            return ret;
        return cYellow + ret + cOff;
    }

}  // namespace qblocks

