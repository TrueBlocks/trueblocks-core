package utils

/*
#pragma once
#include "acctlib.h"

namespace qblocks {

class CTraverser;
//-----------------------------------------------------------------------
typedef bool (*TRAVERSERFUNC)(CTraverser* trav, void* data);
typedef size_t (*COUNTERFUNC)(CTraverser* trav, void* data);

//-----------------------------------------------------------------------
inline bool noopFunc(CTraverser* trav, void* data) {
    return true;
}

//-----------------------------------------------------------------------
extern bool filterByRange(CTraverser* trav, void* data);

typedef map<address_t, CMonitor> monitor_map_t;
//-----------------------------------------------------------------------
class CTraverser {
  public:
    bool logging;
    size_t index;
    size_t nProcessed;
    string_q searchType;
    CMonitor* curMonitor;
    monitor_map_t monitorMap;
    CTraverser(const string_q& o) : index(0), nProcessed(0), searchType(o) {
        logging = !isTestMode();
        curMonitor = nullptr;
        filterFunc = filterByRange;
    }
    virtual ~CTraverser(void) {
    }
    friend ostream& operator<<(ostream& os, const CTraverser& it);

  public:
    TRAVERSERFUNC filterFunc = nullptr;
    COUNTERFUNC counterFunc = nullptr;
    TRAVERSERFUNC preFunc = nullptr;
    TRAVERSERFUNC postFunc = nullptr;
    TRAVERSERFUNC displayFunc = nullptr;
    TRAVERSERFUNC dataFunc = nullptr;
    const CAppearance_mon* app = nullptr;
    CBlock block;
    CTransaction trans;

    bool traverse(const CAppearanceArray_mon& apps, void* data);

  private:
    CTraverser(void) = delete;
};

}  // namespace qblocks

#include "traverser.h"

namespace qblocks {

//-----------------------------------------------------------------------
bool CTraverser::traverse(const CAppearanceArray_mon& apps, void* data) {
    // Prepare the export...
    if (preFunc && !(*preFunc)(this, data))
        return false;

    for (index = 0; index < apps.size() && !shouldQuit(); index++) {
        app = &apps[index];
        trans = CTransaction();  // reset
        bool passedFilter = !filterFunc || (*filterFunc)(this, data);
        if (passedFilter) {
            if (dataFunc && !(*dataFunc)(this, data))
                return (!postFunc || (*postFunc)(this, data)) && false;
            if (displayFunc && !(*displayFunc)(this, data))
                return (!postFunc || (*postFunc)(this, data)) && false;
            nProcessed += (counterFunc ? (*counterFunc)(this, data) : 1);
        }
    }

    // Cleanup the export...
    if (postFunc && !(*postFunc)(this, data))
        return false;

    return true;
}

//-----------------------------------------------------------------------
bool filterByRange(CTraverser* trav, void* data) {
    return bn_2_Timestamp(trav->app->blk) || shouldQuit();
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CTraverser& it) {
    os << "logging: " << it.logging << endl;
    os << "index: " << it.index << endl;
    os << "nProcessed: " << it.nProcessed << endl;
    os << "searchType: " << it.searchType << endl;
    os << "curMonitor: " << it.curMonitor << endl;
    // os << "monitorMap: " << it.monitorMap << endl;
    return os;
}

}  // namespace qblocks
*/
