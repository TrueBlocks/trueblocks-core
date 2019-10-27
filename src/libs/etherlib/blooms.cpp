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
/*
 * This file was generated with makeClass. Edit only those parts of the code inside
 * of 'EXISTING_CODE' tags.
 */
#include "blooms.h"

namespace qblocks {

    //----------------------------------------------------------------------------------
    bool addAddrToBloom(const address_t& addr, CBloomArray& blooms, size_t maxBits) {
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
    string_q reportBloom(const CBloomArray& blooms) {
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

    //--------------------------------------------------------------------------------
    string_q bloom_2_BitStr(const bloom_t& bl) {
        ostringstream os;
        string_q s1 = substitute(bloom_2_Bits(bl), "0", ".");
        for (size_t i = 0 ; i < 16 ; i++) {
            os << "\t";
            for (size_t j = 0 ; j < 128 ; j = j + 10)
                os << extract(extract(s1, i * 128, 128), j, 10) + " ";
            os << endl;
        }
        return os.str();
    }

    //--------------------------------------------------------------------------------
    string_q bloom_2_ByteStr(const bloom_t& bl) {
        ostringstream os;
        string_q s1 = substitute(substitute(bloom_2_Bytes(bl), "0x", ""), "0", ".");
        for (size_t i = 0 ; i < 4 ; i++) {
            os << "\t";
            for (size_t j = 0 ; j < 128 ; j = j + 10)
                os << extract(extract(s1, i * 128, 128), j, 10) + " ";
            os << endl;
        }
        return os.str();
    }

    //-------------------------------------------------------------------------
    bool compareBlooms(const bloom_t& b1, const bloom_t& b2, string_q& str) {
        if (verbose > 2) {
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
            string_q s1 = substitute(substitute(bloom_2_Bytes(b1), "0x", ""), "0", ".");
            string_q s2 = substitute(substitute(bloom_2_Bytes(b2), "0x", ""), "0", ".");
            for (size_t i = 0 ; i < 4 ; i++) {
                string_q m1, m2;
                for (size_t j = 0 ; j < 128 ; j = j + 10) {
                    m1 += extract(extract(s1, i*128, 128), j, 10) + " ";
                    m2 += extract(extract(s2, i*128, 128), j, 10) + " ";
                }
                str += ("\n\t" + cRed + m1 + cOff + "\n\t" + m2);
            }
        }
        return isBloomHit(b1, b2);
    }

    //-------------------------------------------------------------------------
    string_q compareBlooms(const bloom_t& b1, const bloom_t& b2) {
        string_q ret; compareBlooms(b1, b2, ret); return ret;
    }
}  // namespace qblocks

