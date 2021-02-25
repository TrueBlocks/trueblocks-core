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
#include "etherlib.h"

class CCounter {
public:
    uint64_t blocksSeen{0};
    uint64_t txsSeen{0};
    uint64_t errorsSeen{0};
    uint64_t lastBlock{NOPOS};
    CCounter(void) {}
};

blknum_t start = 0;
blknum_t n = getBlockProgress(BP_CLIENT).client - start;
blknum_t step = 1;
blknum_t progress = 1000;
blknum_t report = 50000;

//----------------------------------------------------------------
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);
    CCounter counter;
    forEveryBlockOnDisc(visitBlock, &counter, start, n, step);
    etherlib_cleanup();
    cout << counter.blocksSeen << "\t" << counter.txsSeen << "\t" << counter.errorsSeen << endl;
    return 1;
}

bool visitBlock(CBlock& block, void* data) {
    CCounter *counter = (CCounter*)data;
    counter->blocksSeen++;
    for (uint32_t tr = 0; tr < block.transactions.size(); tr++) {
        counter->txsSeen++;
        if (!(block.blockNumber % progress)) {
            cerr << block.blockNumber << "\t" << block.transactions[tr].transactionIndex << "\r";
        }
        if (block.transactions[tr].isError)
            counter->errorsSeen++;
    }
    if (!(block.blockNumber % report)) {
        cout << block.blockNumber << "," << counter->txsSeen << "," << counter->errorsSeen << string_q(50,' ') << endl;
        counter->txsSeen = counter->errorsSeen = 0;
    }
    if (!(block.blockNumber % progress)) {
        cerr.flush();
    }
    return true;
}
