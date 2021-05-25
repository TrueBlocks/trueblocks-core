#pragma once

#include "acctlib.h"

//-----------------------------------------------------------------------
extern void start_Log(CTraverser* trav, void* data);
extern void prog_Log(CTraverser* trav, void* data);
extern void end_Log(CTraverser* trav, void* data);
extern bool loadTx_Func(CTraverser* trav, void* data);
extern bool pre_Func(CTraverser* trav, void* data);
extern bool post_Func(CTraverser* trav, void* data);

//-----------------------------------------------------------------------
extern bool app_Display(CTraverser* trav, void* data);
extern bool acct_Display(CTraverser* trav, void* data);
extern bool receipts_Display(CTraverser* trav, void* data);
extern bool logs_Display(CTraverser* trav, void* data);
extern bool traces_Display(CTraverser* trav, void* data);

extern bool acct_Pre(CTraverser* trav, void* data);
extern bool logs_Pre(CTraverser* trav, void* data);

extern bool app_Post(CTraverser* trav, void* data);

class CAppearanceTraverser : public CTraverser {
  public:
    CAppearanceTraverser(void) : CTraverser(cout, "appearances") {
        preFunc = pre_Func;
        postFunc = app_Post;
        dataFunc = noopFunc;
        displayFunc = app_Display;
    }
};

class CReceiptTraverser : public CTraverser {
  public:
    CReceiptTraverser(void) : CTraverser(cout, "receipts") {
        preFunc = pre_Func;
        postFunc = post_Func;
        dataFunc = loadTx_Func;
        displayFunc = receipts_Display;
    }
};

class CLogTraverser : public CTraverser {
  public:
    CLogTraverser(void) : CTraverser(cout, "logs") {
        preFunc = logs_Pre;
        postFunc = post_Func;
        dataFunc = loadTx_Func;
        displayFunc = logs_Display;
    }
};

class CTraceTraverser : public CTraverser {
  public:
    CTraceTraverser(void) : CTraverser(cout, "traces") {
        preFunc = pre_Func;
        postFunc = post_Func;
        dataFunc = loadTx_Func;
        displayFunc = traces_Display;
    }
};

class CTransactionTraverser : public CTraverser {
  public:
    CTransactionTraverser(void) : CTraverser(cout, "txs") {
        preFunc = acct_Pre;
        postFunc = post_Func;
        dataFunc = loadTx_Func;
        displayFunc = acct_Display;
    }
};
