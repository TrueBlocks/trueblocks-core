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
#include "basetypes.h"
#include "basenode.h"
#include "fielddata.h"
#include "conversions.h"
#include "logging.h"

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
    return trim(substitute(ret, "  ", " "));
}

//--------------------------------------------------------------
string_q fieldTypeName(uint64_t type) {
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
    os << item.isHidden() << "\t";
    os << fieldTypeStr(item.getType());
    return os;
}

//-----------------------------------------------------------------------
void manageFields(const string_q& listIn, bool show) {
    // LOG4("Entry: manageFields");
    string_q list = substitute(listIn, " ", "");
    while (!list.empty()) {
        string_q fields = nextTokenClear(list, '|');
        string_q cl = nextTokenClear(fields, ':');
        // LOG4("class: " + cl + " fields: " + fields);
        CBaseNode* item = createObjectOfType(cl);
        while (item && !fields.empty()) {
            string_q fieldName = nextTokenClear(fields, ',');
            if (fieldName == "all") {
                if (show) {
                    // LOG4("show " + fieldName);
                    item->getRuntimeClass()->showAllFields();
                } else {
                    // LOG4("hide " + fieldName);
                    item->getRuntimeClass()->hideAllFields();
                }
            } else if (fieldName == "none") {
                if (show) {
                    // LOG4("show " + fieldName);
                    item->getRuntimeClass()->hideAllFields();
                } else {
                    // LOG4("hide " + fieldName);
                    item->getRuntimeClass()->showAllFields();
                }
            } else {
                CFieldData* f = item->getRuntimeClass()->findField(fieldName);
                if (f) {
                    // LOG4((show ? "show " : "hide ") + fieldName);
                    f->setHidden(!show);
                } else {
                    // LOG4("field not found: " + fieldName);
                }
            }
        }
        delete item;
    }
}

//-----------------------------------------------------------------------
void manageFields(const string_q& formatIn) {
    // TODO(tjayrush): This can be removed when shipped
    string_q check = formatIn;
    replaceAll(check, " {", "{");
    replaceAll(check, "\t{", "{");
    check = substitute(check, "[{", "`");
    size_t cntOn = countOf(check, '`');
    check = substitute(substitute(check, "`", ""), "}]", "`");
    if (countOf(check, '`') != cntOn) {
        LOG_ERR("Mismatched brackets in format string: ", formatIn);
        return;
    }
    string_q fields;
    string_q format = substitute(substitute(formatIn, "{", "<field>"), "}", "</field>");
    string_q cl = nextTokenClear(format, ':');
    while (contains(format, "<field>"))
        fields += toLower(snagFieldClear(format, "field") + ",");
    manageFields(cl + ":all", false);
    manageFields(cl + ":" + fields, true);
}

}  // namespace qblocks
