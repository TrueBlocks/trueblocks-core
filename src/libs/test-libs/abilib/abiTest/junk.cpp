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
#include "junk.h"

//-------------------------------------------------------------------------
IMPLEMENT_NODE(CJunk, CBaseNode);
CJunk::CJunk(void) {
    static bool been_here = false;
    if (!been_here) {
        ADD_FIELD(CJunk, "array1", T_OBJECT|TS_ARRAY, 1001);
        been_here = true;
    }
}
bool CJunk::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    return true;
}
string_q CJunk::getValueByName(const string_q& fieldName) const {
    if (fieldName == "array1") {
        size_t cnt = array1.size();
        if (endsWith(fieldName, "Cnt"))
            return uint_2_Str(cnt);
        if (!cnt) return "";
        string_q retS;
        for (size_t i = 0 ; i < cnt ; i++) {
            retS += array1[i].Format();
            retS += ((i < cnt - 1) ? ",\n" : "\n");
        }
        return retS;
    }
    return "";
}
void CJunk::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    ctx << toJson();
    return;
}
