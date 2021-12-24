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
#include "basetypes.h"
#include "conversions.h"

namespace qblocks {

extern bool loadNamesPrefunds(void);
extern bool loadPrefundBals(void);
extern void clearPrefundBals(void);
typedef std::__1::pair<const qblocks::address_t, qblocks::wei_t> PrefundItem;
typedef bool (*PREFUNDFUNC)(const PrefundItem& prefund, void* data);
extern bool forEveryPrefund(PREFUNDFUNC func, void* data);
extern wei_t prefundAt(const address_t& addr);

}  // namespace qblocks
