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

/*-------------------------------------------------------------------------
 * Name:    Simple
 * Purpose: To provide the easiest introduction to the TrueBlocks library.
 *          Simply request a block from Infura and print it to the screen.
 *------------------------------------------------------------------------*/
int main(int argc, const char* argv[]) {
    // Initialize the system and tell it where to find the blockchain data.
    etherlib_init(quickQuitHandler);

    // Request the 3,500,000th block from Infura. Store it in 'block'...
    CBlock block;
    getBlock(block, 3500000);

    // ...print the block to the screen
    cout << block << "\n";

    return 0;
}
