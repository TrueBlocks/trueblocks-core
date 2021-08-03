#pragma once

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
