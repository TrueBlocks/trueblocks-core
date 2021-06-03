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
#include "etherlib.h"

//----------------------------------------------------------------
// Visit every 500th block between the first and the 100,000th
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);
    forEveryBlock(visitBlock, nullptr, 0, 100000, 500);
    etherlib_cleanup();

    return 0;
}

//----------------------------------------------------------------
// Called by forEveryBlock
bool visitBlock(CBlock& block, void* data) {
    return forEveryTransactionInBlock(visitTransaction, data, block);
}

//----------------------------------------------------------------
// Called by forEveryTransactionInBlock, prints the transaction to screen
bool visitTransaction(CTransaction& trans, void* data) {
    cout << trans << endl;
    return true;
}
