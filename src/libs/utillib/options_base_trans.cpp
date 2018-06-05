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
#include "basetypes.h"
#include "options_base.h"
#include "database.h"
#include "sfstring.h"

namespace qblocks {

    //--------------------------------------------------------------------------------
    void COptionsTransList::Init(void) {
        queries = "";
    }

    //--------------------------------------------------------------------------------
    SFString COptionsTransList::parseTransList(const SFString& argIn) {

        SFString arg = argIn;
        if (arg.startsWith("0x")) {

            if (arg.Contains(".")) {

                SFString hash = nextTokenClear(arg, '.');
                if (!isHash(hash) || !isNumeral(arg))
                    return "The argument '" + argIn + "' is not properly formatted.";
                queries += (hash + "." + arg + "|");  // blockHash.transID

            } else if (isHash(arg)) {
                queries += (arg + "|");  // transHash

            } else {
                SFString ret = "The argument '" + argIn + "' is not properly formatted.";
                if (arg.length() != 66)
                    ret += " Is it the right length?";
                return ret;
            }

        } else if (arg.Contains(".")) {
            SFString blockNum = nextTokenClear(arg, '.');
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
    SFString COptionsTransList::toString(void) const {
        return queries;
    }

}  // namespace qblocks
