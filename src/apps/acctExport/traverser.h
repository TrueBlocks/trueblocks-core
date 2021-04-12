#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
/*
 * Parts of this file were generated with makeClass. Edit only those parts of the code
 * outside of the BEG_CODE/END_CODE sections
 */
#include "pinlib.h"

class COptions;
class CTraverser;
//-----------------------------------------------------------------------
typedef bool (*TRAVERSERFUNC)(CTraverser* trav, void* data);
typedef enum { TR_PROGRESS_CACHE = 1, TR_PROGRESS_NODE = 2 } TraverserLog;

//-----------------------------------------------------------------------
extern void start_Log(CTraverser* trav, void* data);
extern void prog_Log(CTraverser* trav, void* data, TraverserLog mode);
extern void end_Log(CTraverser* trav, void* data);
extern bool rangeFilter(CTraverser* trav, void* data);
extern bool loadData(CTraverser* trav, void* data);
inline bool noopFunc(CTraverser* trav, void* data) {
    return true;
}
extern bool pre_Func(CTraverser* trav, void* data);
extern bool post_Func(CTraverser* trav, void* data);

//-----------------------------------------------------------------------
class CTraverser {
  public:
    const COptions* options = nullptr;
    ostream& os;
    string_q op;
    blknum_t lastExpBlock;
    size_t index;
    bool logging;
    size_t nProcessed;
    bool inCache1;
    CTraverser(const COptions* opt, ostream& osIn, const string_q& o)
        : options(opt), os(osIn), op(o), lastExpBlock(NOPOS), index(0), nProcessed(0), inCache1(false) {
        logging = !isTestMode() || getEnvStr("FORCE_LOGGING") == "true";
    }

  public:
    TRAVERSERFUNC preFunc = pre_Func;
    TRAVERSERFUNC filterFunc = rangeFilter;
    TRAVERSERFUNC displayFunc = noopFunc;
    TRAVERSERFUNC postFunc = post_Func;
    TRAVERSERFUNC dataFunc = noopFunc;
    const CAppearance_base* app = nullptr;
    CBlock block1;
    CTransaction trans1;

  private:
    CTraverser(void) = delete;
};
typedef vector<CTraverser> CTraverserArray;

//-----------------------------------------------------------------------
extern bool forEveryAppearance(const CTraverserArray& traversers, const CAppearanceArray_base& apps, void* data);
