#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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
#include "sfos.h"

namespace qblocks {

//----------------------------------------------------------------------------
class CExportOptions {
  public:
    size_t lev, spcs;
    bool noFrst;
    char tab, nl;
    bool quoteNums;
    bool hexNums;
    bool hashesOnly;
    bool colored;
    bool asEther;
    bool asDollars;
    bool asWei;
    bool isParity;
    CNameValueMap fmtMap;
    CExportOptions(void) {
        noFrst = false;
        lev = 0;
        spcs = 2;
        tab = ' ';
        nl = '\n';
        quoteNums = false;
        hexNums = false;
        hashesOnly = false;
        colored = false;
        asEther = false;
        asDollars = false;
        asWei = true;
        isParity = false;
    }
};
extern CExportOptions& expContext(void);
extern void incIndent(void);
extern void decIndent(void);
extern string_q indent(void);
}  // namespace qblocks
