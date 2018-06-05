#pragma once
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

namespace qblocks {

    using timestamp_t = int64_t;
#define isHex(cc) (isdigit(cc) || (cc) == 'x' || ((cc) >= 'a' && (cc) <= 'f') || ((cc) >= 'A' && (cc) <= 'F'))
    inline bool isHexStr(const SFString& str) {
        if (!str.startsWith("0x"))
            return false;
        for (size_t i = 2 ; i < str.length() ; i++)
            if (!isHex(str[(int)i]))
                return false;
        return true;
    }
}  // namespace qblocks
