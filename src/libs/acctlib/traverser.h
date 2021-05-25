#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "acctlib.h"

namespace qblocks {

class CTraverser;
//-----------------------------------------------------------------------
typedef bool (*TRAVERSERFUNC)(CTraverser* trav, void* data);

//-----------------------------------------------------------------------
extern bool rangeFilter(CTraverser* trav, void* data);
inline bool noopFunc(CTraverser* trav, void* data) {
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
    TRAVERSERFUNC preFunc = nullptr;
    TRAVERSERFUNC postFunc = nullptr;
    TRAVERSERFUNC displayFunc = nullptr;
    TRAVERSERFUNC dataFunc = nullptr;
    const CAppearance_base* app = nullptr;
    CBlock block;
    CTransaction trans;

  private:
    CTraverser(void) = delete;
};
typedef vector<CTraverser> CTraverserArray;

//-----------------------------------------------------------------------
extern bool forEveryAppearance(const CTraverserArray& traversers, const CAppearanceArray_base& apps, void* data);

}  // namespace qblocks
