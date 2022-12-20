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

extern const char* STR_PATH_FILE;
extern const char* STR_HEADER2;
extern string_q convert2(const string_q& in, map<string_q, string_q>& map);
//------------------------------------------------------------------------------------------------------------
bool COptions::handle_sdk(void) {
    CToml config(rootConfigToml_makeClass);
    bool enabled = config.getConfigBool("enabled", "sdk", false);
    if (isTestMode() || !enabled) {
        LOG_WARN("Skipping sdk generation...");
        return true;
    }

    LOG_INFO(cYellow, "handling sdk generation...", cOff);

    if (!handle_sdk_types()) {
        return false;
    }

    if (!handle_sdk_paths()) {
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------------------------------------
bool sortByDataModelName2(const CClassDefinition& c1, const CClassDefinition& c2) {
    return c1.class_name < c2.class_name;
}

//------------------------------------------------------------------------------------------------------------
bool COptions::handle_sdk_types(void) {
    sort(dataModels.begin(), dataModels.end(), sortByDataModelName2);
    cerr << "Generating types..." << dataModels.size() << endl;
    for (auto model : dataModels) {
        string_q className = model.class_name;
        string_q fileName = className + ".ts";
        replace(fileName, "C", "");
        fileName = firstLower(fileName);
        fileName = substitute(substitute(fileName, "appearanceDisplay", "appearance"), "logEntry", "log");
        cout << model.class_name << endl << fileName << endl << endl;
    }
    return true;
}

//------------------------------------------------------------------------------------------------------------
bool COptions::handle_sdk_paths(void) {
    for (auto ep : endpointArray) {
        CCommandOptionArray params;
        for (auto option : routeOptionArray) {
            if (option.api_route == ep.api_route && option.isChifraRoute(false)) {
                params.push_back(option);
            }
        }
        ep.params = &params;

        if (!ep.api_route.empty()) {
            ostringstream filename;
            filename << sdkPath << "src/paths/" << ep.api_route << ".ts";
            // cerr << filename.str() << ": " << fileExists(filename.str()) << endl;

            map<string_q, string_q> imports;
            ostringstream rets;
            CStringArray types;
            getReturnTypes(ep, types);
            for (auto type : types) {
                if (rets.str() != "" && rets.str() != "void") {
                    rets << " | ";
                }
                rets << firstUpper(type) << "[]";
                imports[toLower(type)] = firstUpper(type);
            }

            ostringstream pp;
            if (isApiRoute(ep.api_route) && ep.params) {
                for (auto p : *((CCommandOptionArray*)ep.params)) {
                    if (!p.is_visible_docs) {
                        continue;
                    }
                    pp << "    " << substitute(toCamelCase(p.longName), "deleteme", "delete")
                       << (p.is_required ? "" : "?") << ": " << convert2(p.data_type, imports) << "," << endl;
                }
            }

            ostringstream tts;
            for (auto imp : imports) {
                if (tts.str() != "") {
                    tts << ", ";
                }
                string_q v = imp.second;
                tts << v;
            }

            string_q out = STR_HEADER2;
            if (ep.api_route != "daemon" && ep.api_route != "explore") {
                string_q prop = toProper(ep.api_route);
                string_q low = toLower(ep.api_route);
                out += STR_PATH_FILE;
                replaceAll(out, "[{ROUTE}]", prop);
                replaceAll(out, "[{LOWER}]", low);
                replaceAll(out, "[{RETTYPE}]", trim(rets.str(), '\n'));
                replaceAll(out, "[{TYPES}]", trim(tts.str(), '\n'));
                replaceAll(out, "[{PARAMS}]", pp.str());
            } else {
                out += "//\n// The " + ep.api_route + " tool is not available in the SDK.\n//\n";
            }
            stringToAsciiFile(filename.str(), out);
        }

        counter.cmdCount += params.size();
        counter.routeCount++;
    }

    LOG_INFO(cYellow, "makeClass --sdk", cOff, " processed ", counter.routeCount, "/", counter.cmdCount, " paths ",
             " (changed ", counter.nProcessed, ").", string_q(40, ' '));

    return false;
}

//------------------------------------------------------------------------------------------------------------
string_q convert2(const string_q& in, map<string_q, string_q>& map) {
    string_q ret = in;
    if (contains(ret, "list<")) {
        ret = substitute(ret, "list<", "");
        ret = substitute(ret, ">", "");
        ret += "[]";
    }
    if (startsWith(ret, "enum[") && contains(ret, "[]")) {
        ret = "string[]";
    }
    ret = substitute(ret, "addr[]", "address[]");
    ret = substitute(ret, "<boolean>", "boolean");
    ret = substitute(ret, "<uint64>", "uint64");
    ret = substitute(ret, "<blknum>", "blknum");
    ret = substitute(ret, "<string>", "string");
    ret = substitute(ret, "<double>", "double");
    ret = substitute(ret, "<address>", "address");
    ret = substitute(ret, "tx_id", "txId");
    if (!contains(ret, "enum") && !contains(ret, "string") && ret != "boolean") {
        string_q val = substitute(ret, "[]", "");
        map[toLower(val)] = val;
    }
    if (contains(ret, "enum[")) {
        ret = substitute(ret, "enum[", "'");
        ret = substitute(ret, "]", "'");
        ret = substitute(ret, "|", "' | '");
    }
    return ret;
}

//------------------------------------------------------------------------------------------------------------
const char* STR_PATH_FILE =
    "import * as ApiCallers from '../lib/api_callers';\n"
    "import { [{TYPES}] } from '../types';\n"
    "\n"
    "export function get[{ROUTE}](\n"
    "  parameters?: {\n"
    "[{PARAMS}]    chain: string,\n"
    "    noHeader?: boolean,\n"
    "    fmt?: string,\n"
    "    verbose?: boolean,\n"
    "    logLevel?: number,\n"
    "    wei?: boolean,\n"
    "    ether?: boolean,\n"
    "    dollars?: boolean,\n"
    "    raw?: boolean,\n"
    "    toFile?: boolean,\n"
    "  },\n"
    "  options?: RequestInit,\n"
    ") {\n"
    "  return ApiCallers.fetch<[{RETTYPE}]>(\n"
    "    { endpoint: '/[{LOWER}]', method: 'get', parameters, options },\n"
    "  );\n"
    "}\n";

const char* STR_HEADER2 =
    "/* eslint object-curly-newline: [\"error\", \"never\"] */\n"
    "/*\n"
    " * This file was generated with makeClass --sdk. Do not edit it.\n"
    " */\n";
