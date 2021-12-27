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
// NOTE: This file has a lot of NOLINT's in it. Because it's someone else's code, I wanted
// to be conservitive in changing it. It's easier to hide the lint than modify the code

#include "exportcontext.h"
#include "logging.h"

namespace qblocks {

extern string_q getConfigPath(const string_q& part);

//---------------------------------------------------------------------------
// TODO: These singletons are used throughout - it doesn't appear to have any downsides.
// TODO: Assuming this is true, eventually we can remove this comment.
static CExportContext expC;

//---------------------------------------------------------------------------
CExportContext& expContext(void) {
    return expC;
}

//---------------------------------------------------------------------------
CExportContext::CExportContext(void) {
    lev = 0;
    spcs = 2;
    tab = ' ';
    nl = '\n';
    quoteNums = false;
    quoteKeys = true;
    endingCommas = false;
    hexNums = false;
    hashesOnly = false;
    asEther = false;
    asDollars = false;
    asWei = true;
    tsMemMap = nullptr;
    tsCnt = 0;
    exportFmt = (isApiMode() ? API1 : TXT1);
}

//---------------------------------------------------------------------------
void indent(void) {
    expC.lev++;
}

//---------------------------------------------------------------------------
void unindent(void) {
    expC.lev--;
}

//---------------------------------------------------------------------------
string_q indentStr(void) {
    return string_q(expC.spcs * expC.lev, expC.tab);
}

//---------------------------------------------------------------------------
bool isJson(void) {
    return (expC.exportFmt == JSON1 || expC.exportFmt == API1 || expC.exportFmt == NONE1);
}

}  // namespace qblocks
