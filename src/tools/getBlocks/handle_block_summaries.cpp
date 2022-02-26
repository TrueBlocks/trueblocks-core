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
#include "options.h"

//---------------------------------------------------------------------------
bool COptions::handle_block_summaries(blknum_t start, blknum_t nBlocks) {
    bool isText = (expContext().exportFmt & (TXT1 | CSV1));
    blknum_t end = (start < nBlocks ? 0 : start - nBlocks);
    for (blknum_t b = start; b > end; b--) {
        CBlock block;
        getBlockLight(block, b);
        if (!firstOut) {
            if (!isText)
                cout << ",";
            cout << endl;
        }
        string_q fmt = expContext().fmtMap["format"];
        fmt = substitute(fmt, "[{UNCLE_COUNT}]", uint_2_Str(getUncleCount(block.blockNumber)));
        cout << block.Format(fmt);
        firstOut = false;
    }
    return true;
}
