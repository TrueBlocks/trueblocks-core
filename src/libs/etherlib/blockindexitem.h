#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
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

class CBlockIndexItem {
public:
    uint32_t bn;
    uint32_t ts;
    uint16_t tx;
    CBlockIndexItem(void) { bn = ts = 0; tx = 0; }
};
typedef vector<CBlockIndexItem> CBlockIndex;

inline bool operator<(const CBlockIndexItem& i1, const CBlockIndexItem& i2) {
    return i1.bn < i2.bn;
}

} // namespace
