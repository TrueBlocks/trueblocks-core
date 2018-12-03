/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
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
#include "blockoptions.h"

static uint64_t findBlockNumByHash(const hash_t& hash, void *data);
//--------------------------------------------------------------------------------
CBlockOptions::CBlockOptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
void CBlockOptions::Init(void) {
    blocks.hashFind = findBlockNumByHash;
}

//--------------------------------------------------------------------------------
uint64_t findBlockNumByHash(const hash_t& hash, void *data) {
    ASSERT(isHash(hash));
    CBlock block;
    getBlock(block, hash);  // getBlock returns true if it has transactions and false otherwise
    if (block.hash != hash) {
        cerr << "Block hash '" << hash << "' does not appear to be a valid block hash. Quitting...";
        exit(0);
    }
    return block.blockNumber;
}

//--------------------------------------------------------------------------------
bool blockNumToString(uint64_t num, void *data) {
    if (num != NOPOS) {
        string_q *str = (string_q*)data;  // NOLINT
        *str += (uint_2_Str(num) + "|");
    }
    return true;
}

//--------------------------------------------------------------------------------
string_q CBlockOptions::getBlockNumList(void) {
    string_q ret;
    blocks.hashFind = findBlockNumByHash;
    blocks.forEveryBlockNumber(blockNumToString, &ret);
    return ret;
}

//--------------------------------------------------------------------------------
string_q CHistoryOptions::getDispBal(blknum_t bn, biguint_t bal, bool asData) {
    // make finding the dollar value quicker (if we need it)
    if (expContext().asDollars && (timestampMap[bn] == (timestamp_t)0)) {
        CBlock blk;
        getBlock(blk, bn);
        timestampMap[bn] = blk.timestamp;
    }

    ostringstream os;
    if (expContext().asEther) {
        os << wei_2_Ether(bal);
    } else if (expContext().asDollars) {
        string_q s = displayDollars(timestampMap[bn], bal);
        if (asData) {
            os << substitute(s, ",", "");
        } else {
            os << padLeft("$" + s, 14);
        }
    } else {
        os << bal;
    }
    return os.str();
}

//--------------------------------------------------------------------------------
bool CHistoryOptions::hasHistory(void) const {
    if (isTestMode())  // we don't report this error during testing
        return true;

    if (nodeHasBalances())
        return false;

    blknum_t n_blocks = getGlobalConfig()->getConfigInt("history", "n_blocks", 250);
    return (newestBlock - oldestBlock) < n_blocks;
}
