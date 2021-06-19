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
#include "etherlib.h"

namespace qblocks {

//----------------------------------------------------------------
struct CMonitoredAppearance {
    CMonitoredAppearance(void) {
        blk = txid = 0;
    }

  public:
    uint32_t blk;
    uint32_t txid;
    CMonitoredAppearance(uint32_t b, uint32_t t) : blk(b), txid(t) {
    }
    CMonitoredAppearance(const string_q& b, const string_q& t)
        : blk((uint32_t)str_2_Uint(b)), txid((uint32_t)str_2_Uint(t)) {
    }
    CMonitoredAppearance(string_q& line) {  // NOLINT
        replaceAll(line, ".", "\t");
        if (!contains(line, "\t"))
            return;
        blk = (uint32_t)str_2_Uint(nextTokenClear(line, '\t'));
        txid = (uint32_t)str_2_Uint(nextTokenClear(line, '\t'));
    }
};

//----------------------------------------------------------------
typedef vector<CMonitoredAppearance> CMonitoredAppearanceArray;
typedef bool (*MAPPFUNC)(CMonitoredAppearance& app, void* data);

//----------------------------------------------------------------
inline bool operator<(const CMonitoredAppearance& v1, const CMonitoredAppearance& v2) {
    return ((v1.blk != v2.blk) ? v1.blk < v2.blk : v1.txid < v2.txid);
}

//----------------------------------------------------------------
inline bool sortMonitoredAppearanceReverse(const CMonitoredAppearance& v1, const CMonitoredAppearance& v2) {
    return !(v1 < v2);
}

}  // namespace qblocks
