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
static CRouteArray routes;
#define explorerPath string_q("/Users/jrush/Development/trueblocks-explorer/")

//------------------------------------------------------------------------------------------------------------
bool COptions::handle_js(void) {
    CToml config(configPath("makeClass.toml"));
    bool enabled = config.getConfigBool("enabled", "js", false);
    if (isTestMode() || !enabled) {
        LOG_WARN("Skipping javascript generation...");
        return true;
    }

    LOG_INFO(cYellow, "handling javascript route generation...", cOff);
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
            (!route.path.empty() && !route.helpText.empty()) || contains(route.Format("[{NAME}]"), "Template");
        if (hasHelp) {
            if (!firstRoute)
                jsRouteStream << "," << endl;
            jsRouteStream << substitute(route.Format(STR_ROUTE), "&#44;", ",");
            firstRoute = false;
        }

        bool hasPath = route.path.empty() ? false : !contains(route.path, ":");
        if (hasPath) {
            string_q tmp = route.path;
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
    writeCode(routesTSX);

    return true;
}

//------------------------------------------------------------------------------------------------
const char* STR_TYPE_FILE =
    "import {++TYPES++} from '@modules/types';\n"
    "\n"
    "export declare type [{BASE_PROPER}] = {\n++FIELDS++};\n"
    "export declare type [{BASE_PROPER}]Array = [{BASE_PROPER}]++BRACKETS++;\n";

//------------------------------------------------------------------------------------------------
bool COptions::handle_js_type(const CClassDefinition& classDef) {
    ostringstream fields, types;
    CNameValueMap typeMap;

    CParameterArray allFields;
    for (auto field : classDef.fieldArray) {
        allFields.push_back(field);
    }

    CStringArray extras;
    explode(extras, classDef.extra_fields, '|');
    for (auto name : extras) {
        replaceAll(name, "  ", " ");
        string_q type = nextTokenClear(name, ' ');
        CParameter param(name, type);
        allFields.push_back(param);
    }

    for (auto field : allFields) {
        if (startsWith(field.name, "C"))
            replace(field.name, "C", "");
        if (startsWith(field.type, "C"))
            replace(field.type, "C", "");
        replace(field.type, "bool", "bbb");
        replace(field.type, "uint8", "bbb");
        replaceAll(field.type, "bbb", "boolean");
        replace(field.type, "sgas", "gas");
        replace(field.type, "suint64", "uint64");
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
        typesStr = " " + typesStr + " ";
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
    CStringArray fields = {"path", "hotKey", "helpText"};

    CStringArray lines;
    asciiFileToLines(fn, lines);

    bool first = true;
    for (auto line : lines) {
        if (!first) {
            CRoute route;
            route.parseCSV(fields, line);
            routes.push_back(route);
        }
        first = false;
    }
}

//------------------------------------------------------------------------------------------------------------
const char* STR_MOUSETRAP =
    "Mousetrap.bind('[{HOTKEY}]', function () {\n"
    "  window.location.href = [{NAME}];\n"
    "});";

//------------------------------------------------------------------------------------------------------------
const char* STR_LOCATION = "export const [{NAME}] = '[{PATH}]';";

//------------------------------------------------------------------------------------------------------------
const char* STR_ROUTE =
    "  {\n"
    "    path: [{NAME}],\n"
    "    exact: [{EXACT}],\n"
    "    component: [{COMPONENT}],\n"
    "    helpText: '[{HELPTEXT}]',\n"
    "  }";
