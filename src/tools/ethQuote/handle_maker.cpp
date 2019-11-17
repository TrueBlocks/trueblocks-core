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
#include "options.h"
#include "handle_maker.h"

#define MAKER_V1 "0x729d19f657bd0614b4985cf1d82531c67569197b"
#define PEEK "0x59e02dd7"

//-----------------------------------------------------------------------------
wei_t getUsdFromMakerAt(blknum_t blk) {
    string_q cmd = "[{\"to\": \"[CONTRACT]\", \"data\": \"[INPUT]\"}, \"[BLOCK]\"]";
    replace(cmd, "[CONTRACT]", MAKER_V1);
    replace(cmd, "[INPUT]", PEEK);
    replace(cmd, "[BLOCK]", uint_2_Hex(blk));

    string_q result = callRPC("eth_call", cmd, false).substr(2);
    if (result.length() != 128)
        return 0;

    wei_t value = str_2_BigUint("0x" + result.substr(0, 64));
    bool valid = str_2_Bool(result.substr(127, 1));
    if (valid)
        return value;
    return 0;
}
