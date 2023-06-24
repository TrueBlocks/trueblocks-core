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
#include "traversers.h"
#include "options.h"

//--------------------------------------------------------------------------
CLogTraverser::CLogTraverser(void) : CTraverser("logs") {
    filterFunc = filterByRange;
    postFunc = post_Func;
    dataFunc = loadTx_Func;
    displayFunc = logs_Display;
    counterFunc = logs_Count;
}

//--------------------------------------------------------------------------
CTraceTraverser::CTraceTraverser(void) : CTraverser("traces") {
    filterFunc = filterByRange;
    preFunc = pre_Func;
    postFunc = post_Func;
    dataFunc = loadTx_Func;
    displayFunc = traces_Display;
    counterFunc = traces_Count;
}

//--------------------------------------------------------------------------
CNeighborTraverser::CNeighborTraverser(void) : CTraverser("neighbors") {
    filterFunc = filterByRange;
    preFunc = neighbors_Pre;
    postFunc = post_Func;
}

//--------------------------------------------------------------------------
CTransactionTraverser::CTransactionTraverser(void) : CTraverser("txs") {
    filterFunc = filterByRange;
    preFunc = acct_PreFunc;
    postFunc = post_Func;
    dataFunc = loadTx_Func;
    displayFunc = acct_Display;
}
