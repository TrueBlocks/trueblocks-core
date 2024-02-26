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
const char* STR_TYPESCRIPT_PATH_CODE =
    "/* eslint object-curly-newline: [\"error\", \"never\"] */\n"
    "/* eslint max-len: [\"error\", 160] */\n"
    "/*\n"
    " * This file was generated with makeClass --sdk. Do not edit it.\n"
    " */\n"
    "import * as ApiCallers from '../lib/api_callers';\n"
    "import { [{TYPES}] } from '../types';\n"
    "\n"
    "export function get[{ROUTE}](\n"
    "  parameters?: {\n"
    "[{PARAMS}]\n[{GLOBALS}]  },\n"
    "  options?: RequestInit,\n"
    ") {\n"
    "  return ApiCallers.fetch<[{RETTYPE}]>(\n"
    "    { endpoint: '/[{LOWER}]', method: 'get', parameters, options },\n"
    "  );\n"
    "}\n";

//------------------------------------------------------------------------------------------------------------
string_q toTsType(const string_q& in, map<string_q, string_q>& map) {
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
string_q get_corrected_caps(const string_q& capsIn) {
    string_q x = "fmt|chain|noHeader";
    string_q capsOut = substitute(capsIn, "default", x);
    replace(capsOut, "caching", "cache|decache");
    replace(capsOut, "verbose|", "");
    replace(capsOut, "version|", "");
    replace(capsOut, "noop|", "");
    replace(capsOut, "noColor|", "");
    replace(capsOut, "help|", "");
    return capsOut;
}

//------------------------------------------------------------------------------------------------------------
string_q getGlobalParams(const string_q& s1, const string_q& route) {
    string_q caps = get_corrected_caps(s1);
    CStringArray globals;
    explode(globals, caps, '|');

    ostringstream os;
    for (auto global : globals) {
        os << "    " << global << (global == "chain" ? "" : "?") << ": ";
        if (global == "chain" || global == "fmt") {
            os << "string,\n";
        } else {
            os << "boolean,\n";
        }
        reportOneOption(route, global, "typescript-sdk");
    }
    return os.str();
}

//------------------------------------------------------------------------------------------------------------
bool COptions::handle_sdk_ts_paths(CStringArray& pathsOut) {
    for (auto ep : endpointArray) {
        string_q apiRoute = ep.api_route;

        CStringArray types;
        getReturnTypes(ep, types);
        if (!isApiRoute(apiRoute) || types.empty()) {
            // if there is no route nor any returned data, do nothing
            continue;
        }

        ostringstream filename;
        filename << sdkPath << "typescript/src/paths/" << apiRoute << ".ts";

        CCommandOptionArray members;
        for (auto option : routeOptionArray) {
            if (option.isChifraRoute(false) && option.api_route == apiRoute) {
                members.push_back(option);
            }
        }

        counter.cmdCount += members.size();
        counter.routeCount++;

        ostringstream returns;
        map<string_q, string_q> imports;
        for (auto type : types) {
            if (returns.str() != "" && returns.str() != "void") {
                returns << " | ";
            }
            returns << firstUpper(type) << "[]";
            imports[type] = firstUpper(type);
        }

        ostringstream params;
        for (auto member : members) {
            if (!member.is_visible_docs) {
                continue;
            }
            string_q optionName = toCamelCase(member.longName);
            params << "    " << optionName;
            params << (member.is_required ? "" : "?") << ": ";
            params << toTsType(member.data_type, imports) << ",";
            params << endl;

            if (member.option_type != "positional") {
                reportOneOption(apiRoute, optionName, "typescript-sdk");
            }
        }

        ostringstream typeImports;
        for (auto imp : imports) {
            string_q type = imp.second;
            if (type % "any") {
                continue;
            }
            if (typeImports.str() != "") {
                typeImports << ", ";
            }
            typeImports << type;
        }

        string_q source = STR_TYPESCRIPT_PATH_CODE;
        replaceAll(source, "[{ROUTE}]", toProper(apiRoute));
        replaceAll(source, "[{LOWER}]", toLower(apiRoute));
        replaceAll(source, "[{RETTYPE}]", trim(returns.str(), '\n'));
        replaceAll(source, "[{TYPES}]", trim(typeImports.str(), '\n'));
        replaceAll(source, "[{PARAMS}]", trim(params.str(), '\n'));
        replaceAll(source, "[{GLOBALS}]", getGlobalParams(ep.capabilities, apiRoute));

        pathsOut.push_back(apiRoute);
        writeIfDifferent(filename.str(), source);
    }

    ostringstream log;
    log << cYellow << "makeClass --sdk (typescript)" << cOff;
    log << " processed " << counter.routeCount << "/" << counter.cmdCount;
    log << " paths (changed " << counter.nProcessed << ")." << string_q(40, ' ');
    LOG_INFO(log.str());

    return true;
}
