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
#include "database.h"
#include "exportcontext.h"
#include "options_base.h"
#include "colors.h"
#include "filenames.h"
#include "rpcresult.h"

namespace qblocks {

static ostream& out = cerr;
//--------------------------------------------------------------------------------
bool COptionsBase::usage(const string_q& errMsg) const {
    return false;
}

//--------------------------------------------------------------------------------
string_q colorize(const string_q& strIn) {
    string_q str = strIn;
    while (contains(str, '`')) {
        replace(str, "`", cTeal);
        replace(str, "`", cOff);
    }
    return str;
}

//--------------------------------------------------------------------------------
void errorMessage(const string_q& msg) {
    if (isApiMode()) {
        const char* STR_ERROR_JSON = "{ \"errors\": [ \"[ERRORS]\" ] }\n";
        string_q message = substitute(msg, "$CONFIG", rootConfigToml_trueBlocks);
        if (!contains(message, "[")) {
            message = substitute(message, "|", " ");
        }
        string_q errMsg = substitute(STR_ERROR_JSON, "[ERRORS]", message);
        cout << substitute(errMsg, "`", "");
    } else {
        string_q message = substitute(substitute(msg, "|", "\n  "), "$CONFIG", rootConfigToml_trueBlocks);
        message = colorize(message);
        out << endl
            << cRed << "  Warning: " << cOff << message << (endsWith(msg, '.') ? "" : ".") << " Quitting..." << endl
            << endl;
    }
}

}  // namespace qblocks
