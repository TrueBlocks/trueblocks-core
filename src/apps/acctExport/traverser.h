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
typedef bool (*TRAVERSERFUNC)(const CTraverser* trav, void* data);
typedef enum { TR_PROGRESS_CACHE = 1, TR_PROGRESS_NODE = 2 } TraverserLog;

//-----------------------------------------------------------------------
extern void start_Log(const CTraverser* trav, void* data);
extern void prog_Log(const CTraverser* trav, void* data, TraverserLog mode);
extern void end_Log(const CTraverser* trav, void* data);
extern bool rangeFilter(const CTraverser* trav, void* data);
inline bool noopFunc(const CTraverser* trav, void* data) {
    return true;
}
extern bool defPostFunc(const CTraverser* trav, void* data);

//-----------------------------------------------------------------------
class CTraverser {
  public:
    const COptions* options = nullptr;
    ostream& os;
    string_q op;
    blknum_t lastExpBlock;
    size_t index;
    bool logging;
    CTraverser(const COptions* opt, ostream& osIn, const string_q& o)
        : options(opt), os(osIn), op(o), lastExpBlock(NOPOS), index(0) {
        logging = !isTestMode() || getEnvStr("FORCE_LOGGING") == "true";
    }

  public:
    TRAVERSERFUNC preFunc = noopFunc;
    TRAVERSERFUNC filterFunc = rangeFilter;
    TRAVERSERFUNC displayFunc = noopFunc;
    TRAVERSERFUNC postFunc = defPostFunc;
    const CAppearance_base* app = nullptr;
    const CTransaction* trans = nullptr;

  private:
    CTraverser(void) = delete;
};
typedef vector<CTraverser> CTraverserArray;

//-----------------------------------------------------------------------
extern bool forEveryAppearance(const CTraverserArray& traversers, const CAppearanceArray_base& apps, void* data);
