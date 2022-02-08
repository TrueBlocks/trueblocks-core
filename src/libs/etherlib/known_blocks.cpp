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
#include "node.h"

namespace qblocks {

timestamp_t blockZeroTs(void) {
    static timestamp_t ts = 0;
    if (ts == 0) {
        CBlock block;
        getBlock_light(block, 1);
        // This is a hack, but block zero has a zero timestamp
        // and this is the best we can do on multi-chain. Block
        // zero happens 13 seconds before block one (let's say).
        ts = block.timestamp - 13;
    }
    return ts;
}

blknum_t firstTransactionBlock(void) {
    map<string_q, blknum_t> theMap = {
        make_pair("mainnet", 46147),
    };
    return theMap[getChain()];
}

blknum_t hardForkBlock(const string_q& hf) {
    // This data was taken from Geth ./params/config.go
    // TODO: UnchainedIndex --> This could be part of unchained index
    if (hf == "byzantium") {
        map<string_q, blknum_t> theMap = {
            make_pair("kovan", 5067000),
            make_pair("mainnet", 4370000),
            make_pair("rinkeby", 1035301),
            make_pair("ropsten", 1700000),
        };
        return theMap[getChain()];
    } else if (hf == "constantinople") {
        map<string_q, blknum_t> theMap = {
            make_pair("kovan", 9200000),
            make_pair("mainnet", 7280000),
            make_pair("rinkeby", 3660663),
            make_pair("ropsten", 4230000),
        };
        return theMap[getChain()];
    } else if (hf == "london") {
        map<string_q, blknum_t> theMap = {
            make_pair("goerli", 5062605),  make_pair("kovan", 26741100),   make_pair("mainnet", 12965000),
            make_pair("rinkeby", 8897988), make_pair("ropsten", 10499401),
        };
        return theMap[getChain()];
    }
    return 0;
}

//-----------------------------------------------------------------------
bool isDdos(blknum_t bn) {
    if (getChain() != "mainnet")
        return false;
    return (bn >= 2283440 && bn <= 2718436);
}

}  // namespace qblocks

#if 0
// This data was taken from Geth ./params/config.go
ArrowGlacierBlock
	make_pair("mainnet", 13773000),

BerlinBlock
	make_pair("goerli",  4460644),
	make_pair("kovan",   24770900),
	make_pair("mainnet", 12244000),
	make_pair("rinkeby", 8290928),
	make_pair("ropsten", 9812189),

DAOForkBlock
	make_pair("mainnet", 1920000),

HomesteadBlock
	make_pair("mainnet", 1150000),

IstanbulBlock
	make_pair("goerli", 1561651),
	make_pair("kovan", 14111141),
	make_pair("mainnet", 9069000),
	make_pair("rinkeby", 5435345),
	make_pair("ropsten", 6485846),

MuirGlacierBlock
	make_pair("mainnet", 9200000),
	make_pair("ropsten", 7117117),

PetersburgBlock
	make_pair("kovan", 10255201),
	make_pair("mainnet", 7280000),
	make_pair("rinkeby", 4321234),
	make_pair("ropsten", 4939394),
#endif