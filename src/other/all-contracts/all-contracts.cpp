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

bool visitTrace(CTrace& trace, void* data);
//----------------------------------------------------------------
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);

    HIDE_FIELD(CTraceAction, "input");
    HIDE_FIELD(CTraceAction, "init");
    HIDE_FIELD(CTraceResult, "code");
    blknum_t latest = getLatestBlock_client();
    for (size_t bn = firstTransactionBlock ; bn < latest ; bn++) {
        cout << bn << "\r"; cout.flush();
        CBlock block;
        getBlock(block, bn);
        forEveryTraceInBlock(visitTrace, NULL, block);
    }
    return 1;
}

//----------------------------------------------------------------
bool visitTrace(CTrace& trace, void* data) {
    if (!trace.result.address.empty())
        cout << "deployment," << trace.Format("[{RESULT::ADDRESS}],[{RESULT::GASUSED}]") << endl;
    return true;
}
