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
#include "utillib.h"
#include "options.h"

//------------------------------------------------------------------------------------------------------------
bool COptions::handle_sdk_go(void) {
    string_q goSdkPath1 = getCWD() + "apps/chifra/sdk/";
    string_q goSdkPath2 = getCWD() + "../sdk/go/";

    establishFolder(goSdkPath1);
    establishFolder(goSdkPath2);

    for (auto ep : endpointArray) {
        if (ep.api_route == "") {
            continue;
        }

        string_q package = toLower(ep.api_route) + (toLower(ep.api_route) == "init" ? "Pkg" : "");

        string_q contents = asciiFileToString(getPathToTemplates("blank_sdk.go.tmpl"));
        contents = substitute(contents, "[{PROPER}]", toProper(ep.api_route));
        contents = substitute(contents, "[{LOWER}]", toLower(ep.api_route));
        contents = substitute(contents, "[{PKG}]", package);
        writeIfDifferent(goSdkPath1 + ep.api_route + ".go", contents);

        contents = asciiFileToString(getPathToTemplates("blank_sdk2.go.tmpl"));
        contents = substitute(contents, "[{PROPER}]", toProper(ep.api_route));
        contents = substitute(contents, "[{LOWER}]", toLower(ep.api_route));
        contents = substitute(contents, "[{PKG}]", package);
        writeIfDifferent(goSdkPath2 + ep.api_route + ".go", contents);
    }

    return true;
}
