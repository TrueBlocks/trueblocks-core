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
struct CAppearance_mon {
    CAppearance_mon(void) {
        blk = txid = 0;
    }

  public:
    uint32_t blk;
    uint32_t txid;
    CAppearance_mon(uint32_t b, uint32_t t) : blk(b), txid(t) {
    }
    CAppearance_mon(const string_q& b, const string_q& t)
        : blk((uint32_t)str_2_Uint(b)), txid((uint32_t)str_2_Uint(t)) {
    }
    CAppearance_mon(string_q& line) {  // NOLINT
        replaceAll(line, ".", "\t");
        if (!contains(line, "\t"))
            return;
        blk = (uint32_t)str_2_Uint(nextTokenClear(line, '\t'));
        txid = (uint32_t)str_2_Uint(nextTokenClear(line, '\t'));
    }
};

//----------------------------------------------------------------
typedef vector<CAppearance_mon> CAppearanceArray_mon;
typedef bool (*MONAPPFUNC)(CAppearance_mon& app, void* data);

//----------------------------------------------------------------
inline bool operator<(const CAppearance_mon& v1, const CAppearance_mon& v2) {
    return ((v1.blk != v2.blk) ? v1.blk < v2.blk : v1.txid < v2.txid);
}

//----------------------------------------------------------------
inline bool sortMonitoredAppearanceReverse(const CAppearance_mon& v1, const CAppearance_mon& v2) {
    return !(v1 < v2);
}

}  // namespace qblocks
