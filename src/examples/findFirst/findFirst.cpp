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

extern bool visitAddress(const CAppearance& item, void *data);
//-----------------------------------------------------------------------------------------------
int main(int argc, const char *argv[]) {
    etherlib_init(quickQuitHandler);

    address_t search("0xbb9bc244d798123fde783fcc1c72d3bb8c189413");
    blknum_t start = 1428000;
    for (blknum_t bl = start ; bl < getLastBlock_client() ; bl++) {
        CBlock block;
        getBlock(block, bl);
        if (!block.forEveryUniqueAddress(visitAddress, NULL, &search))
            return 0;
    }
    return 0;
}

//-----------------------------------------------------------------------------------------------
bool visitAddress(const CAppearance& item, void *data) {
    if (item.addr == *(address_t *)data) {
        cout << "Found at " << item << "\n";
        return false; // we're done
    }
    cerr << item << "                    \r";
    cerr.flush();
    return true;
}
