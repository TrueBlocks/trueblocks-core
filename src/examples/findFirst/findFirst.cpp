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
// does as it sounds. It visits every item in a given data structure performing a particular
// function. Here, we're showing an example of showing every address appearance in a block.
// We're searching for the first transaction in which the infamous The DAO (0xbb9...)
// appears. There are many forEvery functions such as forEveryTransactionInBlock,
// forEveryTraceInTransaction, forEveryLogInBlock, etc.
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Each forEvery invocation accepts a pointer to a function and a void* to an arbitrary chunk
// of memory. This memory may be of any type you wish. The function signature of the provided
// function differs for each forEvery type. In this case, the signature is as follows.
//-----------------------------------------------------------------------------------------------
bool visitAddress(const CAppearance& item, void* data) {
    // Have we found the address we're looking for?
    if (item.addr == *reinterpret_cast<address_t*>(data)) {
        cout << "\nFound address at: " << item;
        // Return false to stop the scan
        return false;
    }

    // We didn't find it, but let's report progress
    cerr << item.addr << " ";
    cerr << padNum9(item.bn) << " ";
    cerr << padNum5(item.tx) << " ";
    cerr << item.reason << "                \r";
    cerr.flush();

    // Return true to continue
    return true;
}

//-----------------------------------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    // Initialize the library, provide a control+c handler
    etherlib_init(quickQuitHandler);

    // Best guess of where to start the scan
    blknum_t startBlock = 1428700;

    // The address we're searching for
    address_t theDaoAddr("0xbb9bc244d798123fde783fcc1c72d3bb8c189413");

    // Scan each block from the start until we find the address we're looking for
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
