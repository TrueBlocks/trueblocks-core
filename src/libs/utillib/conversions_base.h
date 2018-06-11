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

extern int64_t toLong(const string_q& str);

    using timestamp_t = int64_t;

    //-------------------------------------------------------------------------
    extern uint64_t toLongU   (const string_q& str);
    extern uint64_t hex2LongU (const SFString& str);
    extern double   str2Double(const string_q& str);
    extern bool     str2Bool  (const string_q& str);

    extern SFString string2Hex(const string_q& str);

#define toUnsigned(a)  toLongU((a))
#define toGas(a)       toLongU((a))

}  // namespace qblocks
