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
enum {
    IS_NONE = (0),
    IS_CUSTOM = (1 << 0),
    IS_PREFUND = (1 << 1),
    IS_CONTRACT = (1 << 2),
    IS_ERC20 = (1 << 3),
    IS_ERC721 = (1 << 4),
    IS_DELETED = (1 << 5),
};

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
    bool disc_2_Name(CAccountName& nm) const;
    bool name_2_Disc(const CAccountName& nm);
    string_q Format(void) const;
};

//-----------------------------------------------------------------------
typedef bool (*NAMEFUNC)(CAccountName& name, void* data);
typedef bool (*NAMEODFUNC)(NameOnDisc* name, void* data);
extern bool forEveryNameOld(NAMEFUNC func, void* data);
extern bool forEveryName(NAMEODFUNC func, void* data);

//-----------------------------------------------------------------------
extern bool loadNames(void);
extern bool loadNamesWithPrefunds(void);
extern bool clearNames(void);
extern bool findName(const address_t& addr, CAccountName& acct);
extern bool findToken(const address_t& addr, CAccountName& acct);
extern bool updateName(const CAccountName& target, const string_q& crud);
extern bool hasName(const address_t& addr);
extern size_t nNames(void);

}  // namespace qblocks
