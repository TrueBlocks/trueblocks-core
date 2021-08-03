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
typedef size_t (*COUNTERFUNC)(CTraverser* trav, void* data);

//-----------------------------------------------------------------------
inline bool noopFunc(CTraverser* trav, void* data) {
    return true;
}

//-----------------------------------------------------------------------
extern bool filterFunc(CTraverser* trav, void* data);

typedef map<address_t, CMonitor> monitor_map_t;
//-----------------------------------------------------------------------
class CTraverser {
  public:
    bool logging;
    size_t index;
    size_t nProcessed;
    string_q operation;
    string_q readStatus;
    CMonitor* curMonitor;
    monitor_map_t monitorMap;
    blkrange_t exportRange;
    CTraverser(const string_q& o) : index(0), nProcessed(0), operation(o) {
        logging = !isTestMode() || getEnvStr("FORCE_LOGGING") == "true";
        readStatus = "Extracting";
        curMonitor = nullptr;
        exportRange = make_pair(0, NOPOS);
        filterFunc = ::filterFunc;
    }
    virtual ~CTraverser(void) {
    }

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
typedef vector<CTraverser> CTraverserArray;

//-----------------------------------------------------------------------
extern bool forEveryAppearance(const CTraverserArray& traversers, const CAppearanceArray_mon& apps, void* data);

class CDynamicTraverser {
  private:
    void* m_handle;
    string_q m_path;

  public:
    explicit CDynamicTraverser(const string_q& lib_path) {
        m_handle = ::dlopen(lib_path.c_str(), RTLD_LAZY);
        m_path = lib_path;
        if (m_handle == nullptr)
            LOG_ERR("Failed to load shared object " + lib_path + ": " + string_q(::dlerror()));
    }
    ~CDynamicTraverser() {
        if (m_handle != nullptr) {
            if (::dlclose(m_handle))  // returns non-zero on failure
                LOG_ERR("Failed to unload the shared object: " + string_q(::dlerror()));
        }
    }
    template <typename FunctionSignature>
    FunctionSignature* get_function(const string_q& func_name) {
        void* func_ptr = ::dlsym(m_handle, func_name.c_str());
        if (func_ptr == nullptr)
            LOG_ERR("Failed to get [func_name:" + func_name + "]: " + ::dlerror());
        return reinterpret_cast<FunctionSignature*>(func_ptr);
    }
    bool is_valid(void) const {
        return m_handle != nullptr;
    }

  private:
    CDynamicTraverser(void) = delete;
    CDynamicTraverser(const CDynamicTraverser&) = delete;
    CDynamicTraverser& operator=(const CDynamicTraverser&) = delete;
};

}  // namespace qblocks
