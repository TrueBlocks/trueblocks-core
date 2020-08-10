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
#include "acctlib.h"
#include "options.h"

blknum_t start = 1268725;
//--------------------------------------------------------------
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);
    CRewardReconcilation::registerClass();

#if 1
    string_q contents = asciiFileToString("uncle_blocks.csv");
    CUintArray blocks;
    explode(blocks, contents, '\n');
    blknum_t prev = start;
    for (auto block : blocks) {
        for (blknum_t bn = prev ; bn < block ; bn++) {
            CRewardReconcilation rec;
            rec.blockNumber = bn;
            rec.baseReward = getBlockReward(bn, false);
            rec.timestamp = getBlockTimestamp(bn);
            if (bn == prev) {
                // This block has an uncle
                rec.uncAddReward = getBlockReward(bn, true) - rec.baseReward;
                uint64_t count = getUncleCount(bn);
                for (size_t i = 0 ; i < count ; i++) {
                    CBlock uncle;
                    getUncle(uncle, bn, i);
                    rec.uncleReward += getUncleReward(bn, uncle.blockNumber);
                }
            }
            cout << rec.Format(CSV_DISPLAY_REWARDRECONCILATION) << endl;
        }
        prev = block;
    }
#else
    wei_t total = 0;
    forEveryBlock(visitBlock, &total, 0, getLatestBlock_client());
#endif

    etherlib_cleanup();
    return 0;
}

//--------------------------------------------------------------
timestamp_t getBlockTimestamp(blknum_t bn) {
    static uint32_t *timestamps = NULL;
    static size_t nTimestamps = 0;
    if (nTimestamps == 0) {
        loadTimestampFile(&timestamps, nTimestamps);
        cerr << "Timestamps loaded..." << endl;
    }
    return bn < nTimestamps ? timestamps[(bn*2)+1] : 0;
}

//--------------------------------------------------------------
bool visitBlock(CBlock& block, void* data) {
    CRewardReconcilation rec(block.blockNumber, block.miner);
    rec.timestamp = getBlockTimestamp(block.blockNumber);
    cout << rec.Format(CSV_DISPLAY_REWARDRECONCILATION) << endl;
    return true;
}

string_q CSV_DISPLAY_REWARDRECONCILATION = substitute(STR_DISPLAY_REWARDRECONCILATION, "\t", ",");
