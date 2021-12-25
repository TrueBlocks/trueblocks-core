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

//-----------------------------------------------------------------------
class NameOnDisc {
  public:
    char tags[30 + 1];
    char address[42 + 1];
    char name[120 + 1];
    char symbol[30 + 1];
    char source[180 + 1];
    char description[255 + 1];
    uint16_t decimals;
    uint16_t flags;
    NameOnDisc(void);
    bool disc_2_Name(CAccountName& nm) const;
    bool name_2_Disc(const CAccountName& nm);
    string_q Format(void) const;
};

//-----------------------------------------------------------------------
typedef bool (*NAMEFUNC)(CAccountName& name, void* data);
typedef bool (*NAMEODFUNC)(NameOnDisc* name, void* data);
extern bool forEveryNameOld(NAMEFUNC func, void* data);
extern bool forEveryNameNew(NAMEODFUNC func, void* data);

//-----------------------------------------------------------------------
extern bool loadNames(bool old);
extern bool clearNames(bool old);
extern bool findName(bool old, const address_t& addr, CAccountName& acct);
extern bool findToken(bool old, const address_t& addr, CAccountName& acct);
extern void addPrefundToNamesMap(bool old, CAccountName& account, uint64_t cnt);
extern size_t nNames(bool old);
extern bool oldNames;

}  // namespace qblocks
