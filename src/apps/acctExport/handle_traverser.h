#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "pinlib.h"

class COptions;
class CTraverser;
//-----------------------------------------------------------------------
typedef bool (*TRAVERSERFUNC)(CTraverser* trav, void* data1);

//-----------------------------------------------------------------------
extern void start_Log(CTraverser* trav, void* data1);
extern void prog_Log(CTraverser* trav, void* data1);
extern void end_Log(CTraverser* trav, void* data1);
extern bool rangeFilter(CTraverser* trav, void* data1);
extern bool loadData(CTraverser* trav, void* data1);
extern bool pre_Func(CTraverser* trav, void* data1);
extern bool post_Func(CTraverser* trav, void* data1);
inline bool noopFunc(CTraverser* trav, void* data1) {
    return true;
}

//-----------------------------------------------------------------------
class CTraverser {
  public:
    bool logging;
    size_t index;
    size_t nProcessed;
    string_q operation;
    string_q readStatus;
    CTraverser(ostream& osIn, const string_q& o) : index(0), nProcessed(0), operation(o) {
        logging = !isTestMode() || getEnvStr("FORCE_LOGGING") == "true";
        readStatus = "Extracting";
    }

  public:
    TRAVERSERFUNC filterFunc = rangeFilter;
    TRAVERSERFUNC preFunc = pre_Func;
    TRAVERSERFUNC postFunc = post_Func;
    TRAVERSERFUNC displayFunc = noopFunc;
    TRAVERSERFUNC dataFunc = loadData;
    const CAppearance_base* app = nullptr;
    CBlock block;
    CTransaction trans;

  private:
    CTraverser(void) = delete;
};
typedef vector<CTraverser> CTraverserArray;

//-----------------------------------------------------------------------
extern bool forEveryAppearance(const CTraverserArray& traversers, const CAppearanceArray_base& apps, void* data);
