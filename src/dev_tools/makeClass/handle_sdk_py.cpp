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
#include "acctlib.h"
#include "options.h"

//------------------------------------------------------------------------------------------------------------
bool COptions::handle_sdk_py(void) {
    CStringArray types, paths;
    if (!handle_sdk_py_types(types)) {
        return false;
    }

    if (!handle_sdk_py_paths(paths)) {
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------------------------------------
bool COptions::handle_sdk_py_types(CStringArray& typesOut) {
    return true;
}

//------------------------------------------------------------------------------------------------------------
bool COptions::handle_sdk_py_paths(CStringArray& pathsOut) {
    for (auto ep : endpointArray) {
        CCommandOptionArray members;
        for (auto option : routeOptionArray) {
            if (option.api_route == ep.api_route && isChifraRoute(option, false)) {
                members.push_back(option);
            }
        }
        ep.members = &members;

        if (!ep.api_route.empty()) {
            ostringstream filename;
            filename << sdkPath << "python/src/_" << ep.api_route << ".py";

            string_q source = asciiFileToString(getPathToTemplates("python.tmpl"));
            if (ep.api_route != "daemon" && ep.api_route != "explore") {
                replaceAll(source, "[{ROUTE}]", ep.api_route);
                string_q firstPos;
                ostringstream pp;
                for (auto p : *((CCommandOptionArray*)ep.members)) {
                    // if (!p.is_visible_docs || !p.is_visible) {
                    //     continue;
                    // }
                    string_q cmd = substitute(toCamelCase(p.longName), "deleteme", "delete");
                    const char* STR_LINE =
                        "    \"[{LONGNAME}]\": {\"hotkey\": \"[{HOTKEY}]\", \"type\": \"[{TYPE}]\"},\n";
                    string_q line = STR_LINE;
                    replace(line, "[{LONGNAME}]", cmd);
                    replace(line, "[{HOTKEY}]", p.hotKey.empty() ? "" : "-" + p.hotKey);
                    replace(line, "[{TYPE}]", p.option_type);
                    if (p.option_type == "positional") {
                        if (firstPos.empty()) {
                            firstPos = p.longName;
                        }
                    } else {
                        pp << line;
                    }
                }
                replace(source, "[{OPTIONS}]", pp.str());
                replace(source, "[{FIRST}]", firstPos);

            } else {
                source = "from . import session\n\n# Not available in the SDK\n";
            }
            writeIfDifferent(filename.str(), source + "\n");
        }

        counter.cmdCount += members.size();
        counter.routeCount++;
    }

    LOG_INFO(cYellow, "makeClass --sdk (python)", cOff, " processed ", counter.routeCount, "/", counter.cmdCount,
             " paths ", " (changed ", counter.nProcessed, ").", string_q(40, ' '));

    return true;
}
