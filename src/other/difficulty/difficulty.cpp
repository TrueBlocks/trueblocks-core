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

#define START 14562065
#define END 14585980

int main(int argc, const char* argv[]) {
    loadEnvironmentPaths();
    etherlib_init(quickQuitHandler);
    for (size_t i = START; i < END; i++) {
        CBlock block;
        getBlockLight(block, i);
        cout << block.blockNumber << "," << block.timestamp << "," << block.difficulty << endl;
        cerr << block.blockNumber << " " << (END - block.blockNumber) << "     \r";
        cerr.flush();
    }
    return 0;
}
