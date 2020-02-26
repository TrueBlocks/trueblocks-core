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
#include "webapi.h"

namespace qblocks {

//--------------------------------------------------------------------------------
string_q getApiKey(const string_q& apiName, const string_q& signup) {
    string_q key = getGlobalConfig("")->getConfigStr("settings", toLower(apiName) + "_key", "<NOT_SET>");
    if (!key.empty() && key != "<NOT_SET>")
        return key;

    char buffer[256];
    bzero(buffer, sizeof(buffer));

    cerr << endl << cRed << "  ***Warning***" << cOff << endl;
    cerr << "  This program needs an api_key from " + apiName + " in order to work. You may get one at" << endl;
    cerr << "  " + signup + ". See our online help file for more information. Please" << endl;
    cerr << "  provide an API key or type 'exit'" << endl;
    cerr << "  > ";
    quickQuitHandler(0);
    return "";
}

}  // namespace qblocks
