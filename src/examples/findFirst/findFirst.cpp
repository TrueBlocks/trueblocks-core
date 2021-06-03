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

//-----------------------------------------------------------------------------------------------
// The following code shows an example of using one of TrueBlocks' forEvery features. forEvery
// does as it sounds. It visits every item in a given data structure. Here, we're showing
// an example of showing every address appearance in a block. We're searching for the first
// transaction in which the infamous The DAO (0xbb9...) appears. There are many forEvery functions
// such as forEveryTransactionInBlock, forEveryTraceInTransaction, forEveryLogInBlock, etc.

//-----------------------------------------------------------------------------------------------
// Every forEvery invocation accepts a void* to a chunk of memory that can be of any type
// you wish and a function. The function signature of that function differs for each forEvery
// type. In this case, the signature must be as follows.
bool visitAddress(const CAppearance& item, void* data) {
    if (item.addr == *reinterpret_cast<address_t*>(data)) {
        cout << "Found at " << item << "\n";
        return false;  // return false to stop the scan
    }
    cerr << item << "                    \r";
    return true;  // return true to continue
}

//-----------------------------------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    // Initialize the library
    etherlib_init(quickQuitHandler);

    // a best guess place to start the scan
    blknum_t startBlock = 1428000;

    // the address we're searching for
    address_t theDaoAddr("0xbb9bc244d798123fde783fcc1c72d3bb8c189413");

    // scan each block from the start until we find the address in an appearance
    for (blknum_t bl = startBlock; bl < getBlockProgress(BP_CLIENT).client; bl++) {
        CBlock block;
        getBlock(block, bl);
        if (!block.forEveryUniqueAppearanceInBlock(visitAddress /* func */, NULL /* filterFunc */,
                                                   &theDaoAddr /* data */))
            return 0;
    }

    etherlib_cleanup();

    return 0;
}
