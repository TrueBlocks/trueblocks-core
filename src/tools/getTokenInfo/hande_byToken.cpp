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

//--------------------------------------------------------------
void COptions::reportByToken(void) {
    uint64_t cnt = 0;

    bool needsNewline = true;

    // For each token contract
    for (auto token : tokens) {
        cout << "\n  For token contract: " << bBlue << token.address << cOff << "\n";

        // For each holder
        for (auto holder : holders) {
            cnt++;

            // For each block
            string_q blocks = getBlockNumList();
            while (!blocks.empty()) {
                blknum_t blockNum = str_2_Uint(nextTokenClear(blocks, '|'));
                if (blockNum < oldestBlock)
                    oldestBlock = blockNum;
                token.abi_spec.loadAbiByAddress(token.address);
                biguint_t bal = str_2_Wei(getTokenBalanceOf(token, holder, blockNum));
                string_q dispBal = getDispBal(newestBlock, bal);
                needsNewline = true;
                if (bal > 0 || !no_zero) {
                    cout << "    Balance for account " << cGreen << holder << cOff;
                    cout << " at block " << cTeal << blockNum << cOff;
                    cout << " is " << cYellow << dispBal << cOff << "\n";
                    needsNewline = false;
                } else {
                    cerr << "    Balance for account " << cGreen << holder << cOff;
                    cerr << " at block " << cTeal << blockNum << cOff;
                    cerr << " is " << cYellow << dispBal << cOff << "           \r";
                }
                cerr.flush();
                cout.flush();
            }
        }
    }
    if (needsNewline)
        cerr << string_q(104, ' ') << "\n";
}
