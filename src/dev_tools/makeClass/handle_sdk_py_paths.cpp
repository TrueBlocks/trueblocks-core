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
extern string_q getGlobalFeature(const string_q& route, const string_q& feature);
string_q getGlobals(const string_q& apiRoute) {
    CStringArray globals;
    globals.push_back("ether");
    globals.push_back("raw");
    globals.push_back("cache");

    ostringstream os;
    for (auto global : globals) {
        string_q g = getGlobalFeature(apiRoute, global);
        if (g.empty())
            continue;
        CStringArray parts;
        explode(parts, g, '|');
        string_q line = "    \"[{LONGNAME}]\": {\"hotkey\": \"[{HOTKEY}]\", \"type\": \"[{TYPE}]\"},";
        ;

        string_q optionName = parts[0];
        replace(line, "[{LONGNAME}]", optionName);
        replace(line, "[{HOTKEY}]", parts[3]);
        replace(line, "[{TYPE}]", parts[2] == "boolean" ? "switch" : "flag");
        os << line << endl;

        reportOneOption(apiRoute, optionName, "python");
    }
    return os.str();
}

//------------------------------------------------------------------------------------------------------------
bool COptions::handle_sdk_py_paths(CStringArray& pathsOut) {
    for (auto ep : endpointArray) {
        string_q apiRoute = ep.api_route;

        CStringArray types;
        getReturnTypes(ep, types);
        if (!isApiRoute(apiRoute) || types.empty()) {
            // if there is no route nor any returned data, do nothing
            continue;
        }

        ostringstream filename;
        filename << sdkPath << "python/src/_" << apiRoute << ".py";

        CCommandOptionArray members;
        for (auto option : routeOptionArray) {
            if (option.isChifraRoute(false) && option.api_route == apiRoute) {
                members.push_back(option);
            }
        }

        counter.cmdCount += members.size();
        counter.routeCount++;

        string_q source = asciiFileToString(getPathToTemplates("python.tmpl"));
        replaceAll(source, "[{ROUTE}]", apiRoute);

        if (apiRoute != "daemon" && apiRoute != "explore") {
            string_q firstPos;
            ostringstream params;
            for (auto p : members) {
                string_q line = "    \"[{LONGNAME}]\": {\"hotkey\": \"[{HOTKEY}]\", \"type\": \"[{TYPE}]\"},\n";
                string_q optionName = substitute(toCamelCase(p.longName), "deleteme", "delete");

                if (!p.is_visible_docs && !contains(optionName, "cache")) {
                    continue;
                }

                replace(line, "[{LONGNAME}]", optionName);
                replace(line, "[{HOTKEY}]", p.hotKey.empty() ? "" : "-" + p.hotKey);
                replace(line, "[{TYPE}]", p.option_type);
                if (p.option_type == "positional") {
                    if (firstPos.empty()) {
                        firstPos = p.longName;
                    }
                } else {
                    params << line;
                    reportOneOption(apiRoute, optionName, "python");
                }
            }

            replace(source, "[{GLOBALS}]", getGlobals(apiRoute));
            replace(source, "[{OPTIONS}]", params.str());
            replace(source, "[{FIRST}]", firstPos);
        }
        writeIfDifferent(filename.str(), source + "\n");
    }

    ostringstream log;
    log << cYellow << "makeClass --sdk (python)" << cOff;
    log << " processed " << counter.routeCount << "/" << counter.cmdCount;
    log << " paths (changed " << counter.nProcessed << ")." << string_q(40, ' ');
    LOG_INFO(log.str());

    return true;
}
