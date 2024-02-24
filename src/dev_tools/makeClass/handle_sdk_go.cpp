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
        // We don't do options here, but once we do, we should report on them to make sure all auto-gen code generates
        // the same thing
        // reportOneOption(apiRoute, optionName, "api");

        if (ep.api_route == "") {
            continue;
        }

        string_q package = toLower(ep.api_route) + (toLower(ep.api_route) == "init" ? "Pkg" : "");

        string_q contents = asciiFileToString(getPathToTemplates("blank_sdk.go.tmpl"));
        contents = substitute(contents, "[{PROPER}]", toProper(ep.api_route));
        contents = substitute(contents, "[{LOWER}]", toLower(ep.api_route));
        contents = substitute(contents, "[{PKG}]", package);
        {
            codewrite_t cw(goSdkPath1 + ep.api_route + ".go", contents);
            cw.nSpaces = 0;
            cw.stripEOFNL = false;
            counter.nProcessed += writeCodeIn(this, cw);
            counter.nVisited++;
        }

        ostringstream fields;
        CCommandOptionArray members;
        for (auto option : routeOptionArray) {
            bool isOne = option.api_route == ep.api_route && option.isChifraRoute(true);
            if (isOne) {
                /*
                  "num": "12210",
                  "group": "tools",
                  "api_route": "slurp",
                  "tool": "ethslurp",
                  "longName": "sleep",
                  "hotKey": "s",
                  "def_val": ".25",
                  "is_visible": true,
                  "is_visible_docs": true,
                  "generate": "gocmd",
                  "option_type": "flag",
                  "data_type": "<double>",
                  "real_type": "double",
                  "go_intype": "float64",
                  "go_flagtype": "Float64VarP",
                  "description": "seconds to sleep between requests"
                */
                fields << "\t" << toProper(option.longName) << " " << option.real_type << endl;
                // cout << option << endl;
            }
        }
        // ep.members = &members;
        // cout << members << endl;

        contents = asciiFileToString(getPathToTemplates("blank_sdk2.go.tmpl"));
        contents = substitute(contents, "[{FIEDLS}]", "");  // fields.str());
        contents = substitute(contents, "[{PROPER}]", toProper(ep.api_route));
        contents = substitute(contents, "[{LOWER}]", toLower(ep.api_route));
        contents = substitute(contents, "[{PKG}]", package);
        {
            codewrite_t cw(goSdkPath2 + ep.api_route + ".go", contents);
            cw.nSpaces = 0;
            cw.stripEOFNL = false;
            counter.nProcessed += writeCodeIn(this, cw);
            counter.nVisited++;
        }
    }

    ostringstream log;
    log << cYellow << "makeClass --sdk (go)" << cOff;
    log << " processed " << counter.routeCount << "/" << counter.cmdCount;
    log << " paths (changed " << counter.nProcessed << ")." << string_q(40, ' ');
    LOG_INFO(log.str());

    return true;
}
