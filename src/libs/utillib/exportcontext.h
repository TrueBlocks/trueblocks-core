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
#include "runtimeclass.h"
#include "sfos.h"
#include "conversions.h"
#include "accountname.h"

namespace qblocks {

typedef map<address_t, CAccountName> CAddressNameMap;

//----------------------------------------------------------------------------
enum format_t { NONE1 = 0, JSON1 = (1 << 1), TXT1 = (1 << 2), CSV1 = (1 << 3), API1 = (1 << 4), YAML1 = (1 << 5) };

//----------------------------------------------------------------------------
class CExportContext {
  public:
    size_t lev, spcs;
    char tab, nl;
    bool quoteNums;
    bool quoteKeys;
    bool endingCommas;
    bool hexNums;
    bool hashesOnly;
    bool asEther;
    bool asDollars;
    bool asWei;
    uint32_t* tsMemMap;
    size_t tsCnt;
    format_t exportFmt;
    CNameValueMap fmtMap;
    CAddressWeiMap prefundMap;
    CAddressNameMap tokenMap;
    CAddressNameMap namesMap;

  public:
    CExportContext(void);
};

//----------------------------------------------------------------------------
extern CExportContext& expContext(void);
extern string_q indentStr(void);
extern void indent(void);
extern void unindent(void);
extern bool isJson(void);

//-----------------------------------------------------------------------
extern bool loadPrefunds(void);
extern wei_t prefundAt(const address_t& addr);

//-----------------------------------------------------------------------
extern bool findToken(const address_t& addr, CAccountName& acct);

}  // namespace qblocks
