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
    searchOpType searchOp;
    CMonitor* curMonitor;
    monitor_map_t monitorMap;
    blkrange_t traverserRange;
    CTraverser(const string_q& o) : index(0), nProcessed(0), searchType(o) {
        logging = !isTestMode() || getEnvStr("FORCE_LOGGING") == "true";
        searchOp = EXTRACT;
        curMonitor = nullptr;
        traverserRange = make_pair(0, NOPOS);
        filterFunc = filterByRange;
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
