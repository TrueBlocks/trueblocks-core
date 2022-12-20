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
extern const char* STR_MOUSETRAP;
extern const char* STR_LOCATION;
extern const char* STR_ROUTE;
extern const char* STR_TYPE_FILE;
extern void loadRoutes(const string_q& fn);

//------------------------------------------------------------------------------------------------------------
static CCommandOptionArray routes;

//------------------------------------------------------------------------------------------------------------
bool COptions::handle_tsx(void) {
    CToml config(rootConfigToml_makeClass);
    bool enabled = config.getConfigBool("enabled", "tsx", false);
    if (isTestMode() || !enabled) {
        LOG_WARN("Skipping typescript generation...");
        return true;
    }

    LOG_INFO(cYellow, "handling typescript route generation...", cOff);
    counter = CCounter();  // reset

    string_q routesCSV = explorerPath + "src/ui/Routes.csv";
    loadRoutes(routesCSV);

    bool firstHotkey = true, firstRoute = true, firstLocation = true;
    string_q current;
    jsRouteStream << "export const routes = [" << endl;
    for (auto route : routes) {
        if (!route.hotKey.empty()) {
            if (!firstHotkey)
                jsHotkeyStream << endl;
            jsHotkeyStream << route.Format(STR_MOUSETRAP);
            firstHotkey = false;
        }

        bool hasHelp =
            (!route.api_route.empty() && !route.description.empty()) || contains(route.Format("[{NAME}]"), "Template");
        if (hasHelp) {
            if (!firstRoute)
                jsRouteStream << "," << endl;
            jsRouteStream << substitute(route.Format(STR_ROUTE), "&#44;", ",");
            firstRoute = false;
        }

        bool hasPath = route.api_route.empty() ? false : !contains(route.api_route, ":");
        if (hasPath) {
            string_q tmp = route.api_route;
            nextTokenClear(tmp, '/');
            tmp = nextTokenClear(tmp, '/');

            if (!firstLocation)
                jsLocationStream << endl;
            if (tmp != current)
                jsLocationStream << endl;
            jsLocationStream << route.Format(STR_LOCATION);
            firstLocation = false;

            current = tmp;
        }
    }
    jsRouteStream << endl << "];" << endl;
    jsHotkeyStream << endl;
    jsLocationStream << endl;

    string_q routesTSX = explorerPath + "src/ui/Routes.tsx";
    writeCodeOut(this, routesTSX);

    return true;
}

//------------------------------------------------------------------------------------------------
const char* STR_TYPE_FILE =
    "import {\n ++TYPES++\n} from '@modules/types';\n"
    "\n"
    "export type [{BASE_PROPER}] = {\n++FIELDS++};\n"
    "export type [{BASE_PROPER}]Array = [{BASE_PROPER}]++BRACKETS++;\n";

//------------------------------------------------------------------------------------------------
bool COptions::handle_tsx_type(const CClassDefinition& classDef) {
    ostringstream fields, types;
    CNameValueMap typeMap;

    CParameterArray allFields;
    for (auto field : classDef.fieldArray)
        allFields.push_back(field);

    for (auto extra : classDef.extraArray)
        allFields.push_back(extra);

    for (auto field : allFields) {
        if (startsWith(field.name, "C"))
            replace(field.name, "C", "");
        if (startsWith(field.type, "C"))
            replace(field.type, "C", "");
        replace(field.type, "bool", "bbb");
        replace(field.type, "uint8", "bbb");
        replaceAll(field.type, "bbb", "boolean");
        replace(field.type, "Value", "string");
        replace(field.type, "TraceAction", "Traceaction");
        replace(field.type, "TraceResult", "Traceresult");
        replace(field.type, "LogEntryArray", "LogentryArray");
        if (field.type == "StringArray")
            field.type = "string++BRACKETS++";
        if (field.name == "status" && classDef.class_name == "CReceipt")
            field.type = "string";
        if (field.name == "components" && classDef.class_name == "CParameter")
            field.type = "string";
        fields << field.Format("  [{NAME}]: [{TYPE}];") << endl;
        if (!startsWith(field.type, "string") && field.type != "boolean")
            typeMap[toLower(field.type)] = field.type;
    }

    for (auto t : typeMap)
        types << t.second << ", ";
    string_q typesStr = trim(trim(types.str(), ' '), ',');
    if (typesStr.size() > 80) {
        replaceAll(typesStr, ", ", ",\n  ");
        typesStr = trim(typesStr, ' ');
        typesStr += ",";
        typesStr = "\n  " + trim(typesStr, ' ') + "\n";
    } else {
        typesStr = " " + typesStr + "";
    }

    string_q out = classDef.Format(STR_TYPE_FILE);
    replaceAll(out, "++TYPES++", typesStr);
    replaceAll(out, "++FIELDS++", fields.str());
    replaceAll(out, "++BRACKETS++", "[]");

    string_q path = classDef.Format(explorerPath + "src/ui/modules/types/[{BASE_PROPER}].tsx");
    stringToAsciiFile(path, out);

    return true;
}

//------------------------------------------------------------------------------------------------------------
void loadRoutes(const string_q& fn) {
    CStringArray fields = {"api_route", "hotKey", "description"};

    CStringArray lines;
    asciiFileToLines(fn, lines);

    bool first = true;
    for (auto line : lines) {
        if (!first) {
            CCommandOption route;
            route.parseCSV(fields, line);
            routes.push_back(route);
        }
        first = false;
    }
}

//------------------------------------------------------------------------------------------------------------
const char* STR_MOUSETRAP =
    "Mousetrap.bind('[{HOTKEY}]', () => {\n"
    "  window.location.href = [{NAME}];\n"
    "});";

//------------------------------------------------------------------------------------------------------------
const char* STR_LOCATION = "export const [{NAME}] = '[{API_ROUTE}]';";

//------------------------------------------------------------------------------------------------------------
const char* STR_ROUTE =
    "  {\n"
    "    path: [{NAME}],\n"
    "    exact: [{EXACT}],\n"
    "    component: [{COMPONENT}],\n"
    "    helpText: '[{DESCRIPTION}]',\n"
    "  }";
