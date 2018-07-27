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
#include "basetypes.h"
#include "version.h"

namespace qblocks {

#define MAJOR 0
#define MINOR 5
#define BUILD 2
#define SUBVERS "alpha"
    //--------------------------------------------------------------------------------
    uint32_t getVersionNum(void) {
        return getVersionNum(MAJOR, MINOR, BUILD);
    }

    //--------------------------------------------------------------------------------
    uint32_t getVersionNum(uint16_t maj, uint16_t min, uint16_t build) {
        return ((maj * 1000000) + (min * 1000) + (build));
    }

    //--------------------------------------------------------------------------------
    string_q getVersionStr(const string_q& sep1, const string_q& sep2) {
        return uint_2_Str(MAJOR) + sep1 + uint_2_Str(MINOR) + sep1 + uint_2_Str(BUILD) + sep2 + SUBVERS;
    }

}  // namespace qblocks
