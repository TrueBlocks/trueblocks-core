/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
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
#include "basenode.h"
#include "fielddata.h"
#include "conversions.h"

namespace qblocks {

//--------------------------------------------------------------
string_q baseTypeName(uint64_t type) {
    string_q ret;
    if (type & TS_NUMERAL)
        ret += (" TS_NUMERAL ");
    if (type & TS_INTEGER)
        ret += (" TS_INTEGER ");
    if (type & TS_STRING)
        ret += (" TS_STRING ");
    if (type & TS_DATE)
        ret += (" TS_DATE ");
    if (type & TS_ARRAY)
        ret += (" TS_ARRAY ");
    if (type & TS_OBJECT)
        ret += (" TS_OBJECT ");
    if (type & TS_POINTER)
        ret += (" TS_POINTER ");
    if (type & TS_BIGNUM)
        ret += (" TS_BIGNUM ");
    if (type & TS_OMITEMPTY)
        ret += (" TS_OMITEMPTY ");
    return trim(substitute(ret, "  ", " "));
}

//--------------------------------------------------------------
string_q fieldTypeName(uint64_t type) {
    type &= uint64_t(~TS_OMITEMPTY);
    switch (type) {
        case T_DATE:
            return "T_DATE";
        case T_TIME:
            return "T_TIME";
        case T_BOOL:
            return "T_BOOL";
        case T_BLOCKNUM:
            return "T_BLOCKNUM";
        case T_NUMBER:
            return "T_NUMBER";
        case T_UNUMBER:
            return "T_UNUMBER";
        case T_DOUBLE:
            return "T_DOUBLE";
        case T_WEI:
            return "T_WEI";
        case T_UINT256:
            return "T_UINT256";
        case T_INT256:
            return "T_INT256";
        case T_GAS:
            return "T_GAS";
        case T_ETHER:
            return "T_ETHER";
        case T_TEXT:
            return "T_TEXT";
        case T_ADDRESS:
            return "T_ADDRESS";
        case T_TIMESTAMP:
            return "T_TIMESTAMP";
        case T_HASH:
            return "T_HASH";
        case T_IPFSHASH:
            return "T_IPFSHASH";
        case T_BLOOM:
            return "T_BLOOM";
        case T_POINTER:
            return "T_POINTER";
        case T_OBJECT:
            return "T_OBJECT";
        case T_OBJECT | TS_ARRAY:
            return "T_OBJECT|TS_ARRAY";
        case T_TEXT | TS_ARRAY:
            return "T_TEXT|TS_ARRAY";
        case T_ADDRESS | TS_ARRAY:
            return "T_ADDRESS|TS_ARRAY";
        default:
            return "Unknown";
    }
    return "Unknown";
}

//--------------------------------------------------------------
string_q fieldTypeStr(uint64_t type) {
    return uint_2_Str(type) + "\t" + fieldTypeName(type) + " " + baseTypeName(type);
}

//--------------------------------------------------------------
ostream& operator<<(ostream& os, const CFieldData& item) {
    os << padRight(item.getName(), 20) << "\t";
    os << item.getID() << "\t";
    // os << item.isHidden() << "\t";
    os << fieldTypeStr(item.getType());
    return os;
}

}  // namespace qblocks
