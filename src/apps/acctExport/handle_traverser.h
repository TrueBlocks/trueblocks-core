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
typedef bool (*TRAVERSERFUNC)(CTraverser* trav, void* data);
typedef enum { TR_PROGRESS_CACHE = 1, TR_PROGRESS_NODE = 2 } TraverserLog;

//-----------------------------------------------------------------------
extern void start_Log(CTraverser* trav, void* data);
extern void prog_Log(CTraverser* trav, void* data);
extern void end_Log(CTraverser* trav, void* data);
extern bool rangeFilter(CTraverser* trav, void* data);
extern bool loadData(CTraverser* trav, void* data);
extern bool pre_Func(CTraverser* trav, void* data);
extern bool post_Func(CTraverser* trav, void* data);
inline bool noopFunc(CTraverser* trav, void* data) {
    return true;
}

//-----------------------------------------------------------------------
class CTraverser {
  public:
    ostream& os;
    string_q operation;
    size_t index;
    bool logging;
    size_t nProcessed;
    bool inCache;
    const COptions* options = nullptr;
    CTraverser(const COptions* opt, ostream& osIn, const string_q& o)
        : os(osIn), operation(o), index(0), nProcessed(0), inCache(false), options(opt) {
        logging = !isTestMode() || getEnvStr("FORCE_LOGGING") == "true";
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
