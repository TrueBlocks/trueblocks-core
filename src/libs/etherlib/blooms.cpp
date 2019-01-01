/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
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

}  // namespace qblocks

