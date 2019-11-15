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

namespace qblocks {

    //--------------------------------------------------------------
    static string_q baseTypeName(uint64_t type) {
        string_q ret;
        if (type & TS_NUMERAL) ret += (" TS_NUMERAL ");
        if (type & TS_STRING)  ret += (" TS_STRING " );
        if (type & TS_DATE)    ret += (" TS_DATE "   );
        if (type & TS_ARRAY)   ret += (" TS_ARRAY "  );
        if (type & TS_OBJECT)  ret += (" TS_OBJECT " );
        if (type & TS_POINTER) ret += (" TS_POINTER ");
        if (type & TS_BIGNUM)  ret += (" TS_BIGNUM " );
        return trim(substitute(ret, "  ", " "));
    }

    //--------------------------------------------------------------
    static string_q typeName(uint64_t type) {

        string_q t("\t");
        if (type == T_DATE)      return uint_2_Str(type) + t + "T_DATE "    + baseTypeName(type);
        if (type == T_TIME)      return uint_2_Str(type) + t + "T_TIME "    + baseTypeName(type);
        if (type == T_BOOL)      return uint_2_Str(type) + t + "T_BOOL "    + baseTypeName(type);
        if (type == T_NUMBER)    return uint_2_Str(type) + t + "T_NUMBER "  + baseTypeName(type);
        if (type == T_DOUBLE)    return uint_2_Str(type) + t + "T_DOUBLE "  + baseTypeName(type);
        if (type == T_WEI)       return uint_2_Str(type) + t + "T_WEI "     + baseTypeName(type);
        if (type == T_GAS)       return uint_2_Str(type) + t + "T_GAS "     + baseTypeName(type);
        if (type == T_ETHER)     return uint_2_Str(type) + t + "T_ETHER "   + baseTypeName(type);
        if (type == T_TEXT)      return uint_2_Str(type) + t + "T_TEXT "    + baseTypeName(type);
        if (type == T_ADDRESS)   return uint_2_Str(type) + t + "T_ADDRESS " + baseTypeName(type);
        if (type == T_TIMESTAMP) return uint_2_Str(type) + t + "T_TIMESPAN "+ baseTypeName(type);
        if (type == T_HASH)      return uint_2_Str(type) + t + "T_HASH "    + baseTypeName(type);
        if (type == T_BLOOM)     return uint_2_Str(type) + t + "T_BLOOM "   + baseTypeName(type);
        if (type == T_POINTER)   return uint_2_Str(type) + t + "T_POINTER " + baseTypeName(type);
        if (type == T_OBJECT)    return uint_2_Str(type) + t + "T_OBJECT "  + baseTypeName(type);

        if (type == (T_OBJECT|TS_ARRAY))  return uint_2_Str(type) + t + "T_OBJECT|TS_ARRAY "  + baseTypeName(type);
        if (type == (T_TEXT|TS_ARRAY))    return uint_2_Str(type) + t + "T_TEXT|TS_ARRAY "    + baseTypeName(type);
        if (type == (T_ADDRESS|TS_ARRAY)) return uint_2_Str(type) + t + "T_ADDRESS|TS_ARRAY " + baseTypeName(type);

        return uint_2_Str(type) + t + "Unknown";
    }

    ostream& operator<<(ostream& os, const CFieldData& item) {
        os << padRight(item.getName(), 20) << "\t";
        os << item.getID() << "\t";
        os << item.isHidden() << "\t";
        os << typeName(item.getType());
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
            CBaseNode *item = createObjectOfType(cl);
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
                    CFieldData *f = item->getRuntimeClass()->findField(fieldName);
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
        string_q fields;
        string_q format = substitute(substitute(formatIn, "{", "<field>"), "}", "</field>");
        string_q cl = nextTokenClear(format, ':');
        while (contains(format, "<field>"))
            fields += toLower(snagFieldClear(format, "field") + ",");
        manageFields(cl + ":all", false);
        manageFields(cl + ":" + fields, true);
    }

}  // namespace qblocks
