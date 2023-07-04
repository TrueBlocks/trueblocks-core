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
