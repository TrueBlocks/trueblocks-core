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
#include "options_base.h"
#include "database.h"

namespace qblocks {

//--------------------------------------------------------------------------------
void COptionsTransList::Init(void) {
    queries = "";
}

//--------------------------------------------------------------------------------
string_q COptionsTransList::parseTransList(const string_q& argIn) {
    string_q arg = argIn;
    if (contains(arg, ".0-to-")) {
        replace(arg, "-to-", ".");
        CUintArray parts;
        explode(parts, arg, '.');
        for (uint64_t i = parts[1]; i < parts[2]; i++) {
            arg = uint_2_Str(parts[0]) + "." + uint_2_Str(i);
            string_q ret = parseTransList(arg);
            if (!ret.empty())
                return ret;
        }
        return "";
    }

    if (startsWith(arg, "0x")) {
        if (contains(arg, ".")) {
            string_q hash = nextTokenClear(arg, '.');
            if (!isHash(hash) || !isNumeral(arg))
                return "The argument '" + argIn + "' is not properly formatted.";
            queries += (hash + "." + arg + "|");  // blockHash.transID

        } else if (isHash(arg)) {
            queries += (arg + "|");  // transHash

        } else {
            string_q ret = "The argument '" + argIn + "' is not properly formatted.";
            if (arg.length() != 66)
                ret += " Is it the right length?";
            return ret;
        }

    } else if (contains(arg, ".")) {
        string_q blockNum = nextTokenClear(arg, '.');
        if (!isNumeral(blockNum) || !isNumeral(arg))
            return "The argument '" + argIn + "' is not properly formatted.";
        queries += (argIn + "|");  // blockNum.transID

    } else {
        return "The argument '" + argIn + "' is not properly formatted.";
    }

    return "";
}

//--------------------------------------------------------------------------------
COptionsTransList::COptionsTransList(void) {
    Init();
}

//--------------------------------------------------------------------------------
string_q COptionsTransList::int_2_Str(void) const {
    return queries;
}

}  // namespace qblocks
