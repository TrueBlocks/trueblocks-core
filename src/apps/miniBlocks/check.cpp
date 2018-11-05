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
#include "options.h"

//--------------------------------------------------------------
void checkMiniBlocks(const COptions& opts, CVisitor& reporter) {
    uint64_t save = verbose;
    verbose = true;
    reporter.deep = opts.deep;
    forEveryMiniBlockInMemory(checkMiniBlock, &reporter, reporter.firstBlock(), reporter.size());
    verbose = save;
    return;
}

//-----------------------------------------------------------------------------------------------
bool checkMiniBlock(CMiniBlock& miniBlock, const CMiniTrans *, void *data) {

    CVisitor *reporter = reinterpret_cast<CVisitor *>(data);
    string_q fn = getBinaryFilename(miniBlock.blockNumber);
    if (fileExists(fn)) {

        // tell user where we are
        cerr << "Checking block: " << miniBlock.blockNumber;
        cerr << " at " << ts_2_Date(miniBlock.timestamp) << " : ";
        if (reporter->deep) {
            CBlock block;
            getBlock(block, miniBlock.blockNumber);
            bool same = (miniBlock == block);
            cerr << (same ? greenCheck : redX) << " ";
            if (!same) {
                cerr << "Block " << miniBlock.blockNumber << " is different than miniBlock. Quitting...\n";
                cerr.flush();
                exit(0);
            }
        }
        cerr << "              \r";
        cerr.flush();

    } else {
        cerr << "Block file '" << fn << "' not found. Quitting...\n";
        cerr.flush();
        exit(0);
    }
    return true;
}
