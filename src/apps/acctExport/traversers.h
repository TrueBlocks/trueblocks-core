#pragma once
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
#include "acctlib.h"
#include "accumulator.h"

//-----------------------------------------------------------------------
extern bool tsRangeFunc(CTraverser* trav, void* data);
extern void start_Log(CTraverser* trav, void* data);
extern void prog_Log(CTraverser* trav, void* data);
extern void end_Log(CTraverser* trav, void* data);
extern bool loadTx_Func(CTraverser* trav, void* data);
extern bool pre_Func(CTraverser* trav, void* data);
extern bool post_Func(CTraverser* trav, void* data);

extern bool app_Post(CTraverser* trav, void* data);
extern bool app_Display(CTraverser* trav, void* data);
class CAppearanceTraverser : public CTraverser {
  public:
    CAppearanceTraverser(void) : CTraverser("appearances") {
        filterFunc = tsRangeFunc;
        preFunc = pre_Func;
        postFunc = app_Post;
        dataFunc = noopFunc;
        displayFunc = app_Display;
    }
};

extern bool receipts_Display(CTraverser* trav, void* data);
class CReceiptTraverser : public CTraverser {
  public:
    CReceiptTraverser(void) : CTraverser("receipts") {
        filterFunc = tsRangeFunc;
        preFunc = pre_Func;
        postFunc = post_Func;
        dataFunc = loadTx_Func;
        displayFunc = receipts_Display;
    }
};

extern bool statements_Display(CTraverser* trav, void* data);
class CStatementTraverser : public CTraverser {
  public:
    CStatementTraverser(void) : CTraverser("statements") {
        filterFunc = tsRangeFunc;
        preFunc = pre_Func;
        postFunc = post_Func;
        dataFunc = loadTx_Func;
        displayFunc = statements_Display;
    }
};

extern bool logs_Display(CTraverser* trav, void* data);
extern size_t logs_Count(CTraverser* trav, void* data);
class CLogTraverser : public CTraverser {
  public:
    CLogTraverser(void) : CTraverser("logs") {
        filterFunc = tsRangeFunc;
        postFunc = post_Func;
        dataFunc = loadTx_Func;
        displayFunc = logs_Display;
        counterFunc = logs_Count;
    }
};

extern bool traces_Display(CTraverser* trav, void* data);
extern size_t traces_Count(CTraverser* trav, void* data);
class CTraceTraverser : public CTraverser {
  public:
    CTraceTraverser(void) : CTraverser("traces") {
        filterFunc = tsRangeFunc;
        preFunc = pre_Func;
        postFunc = post_Func;
        dataFunc = loadTx_Func;
        displayFunc = traces_Display;
        counterFunc = traces_Count;
    }
};

extern bool acct_Display(CTraverser* trav, void* data);
extern bool acct_PreFunc(CTraverser* trav, void* data);
class CTransactionTraverser : public CTraverser {
  public:
    CAccumulator pl;
    CTransactionTraverser(void) : CTraverser("txs") {
        filterFunc = tsRangeFunc;
        preFunc = acct_PreFunc;
        postFunc = post_Func;
        dataFunc = loadTx_Func;
        displayFunc = acct_Display;
    }
    ~CTransactionTraverser(void) {
    }
};

extern bool neighbors_Pre(CTraverser* trav, void* data);
extern size_t neighbors_Count(CTraverser* trav, void* data);
class CNeighborTraverser : public CTraverser {
  public:
    CNeighborTraverser(void) : CTraverser("neighbors") {
        preFunc = neighbors_Pre;
    }
};
