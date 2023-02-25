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
extern string_q convert2(const string_q& in, map<string_q, string_q>& map);
extern string_q type2Key(const string_q& type);

//------------------------------------------------------------------------------------------------------------
bool COptions::handle_sdk_ts(void) {
    CStringArray types, paths;
    if (!handle_sdk_ts_types(types)) {
        return false;
    }

    if (!handle_sdk_ts_paths(paths)) {
        return false;
    }

    types.push_back("Basetypes");
    sort(types.begin(), types.end());
    ostringstream os1;
    for (auto type : types) {
        os1 << "export * from './" << firstLower(type) << "';" << endl;
    }
    string_q typesPath = sdkPath + "typescript/src/types/index.ts";
    writeIfDifferent(typesPath, os1.str());

    sort(paths.begin(), paths.end());
    ostringstream os2;
    for (auto path : paths) {
        os2 << "export * from './" << path << "';" << endl;
    }
    string_q pathsPath = sdkPath + "typescript/src/paths/index.ts";
    writeIfDifferent(pathsPath, os2.str());

    return true;
}

//------------------------------------------------------------------------------------------------------------
bool COptions::handle_sdk_ts_types(CStringArray& typesOut) {
    for (auto model : dataModels) {
        string_q modelName = substitute(model.class_name, "Array", "");
        modelName = firstUpper(modelName);
        replace(modelName, "C", "");
        if (modelName == "Status") {
            modelName = "Config";
        }

        map<string_q, string_q> imports;
        ostringstream fieldStream;
        for (auto field : model.fieldArray) {
            if (endsWith(field.type, "Map") || field.type == "CKeyArray" || field.type == "Value" ||
                field.type == "topic" || startsWith(field.name, "unused") || contains(field.name, "::")) {
                continue;
            }
            bool isArray = contains(field.type, "Array");
            string_q ft = substitute(substitute(substitute(field.type, "Array", ""), "Ptr", ""), "uint8", "bool");
            if (startsWith(field.type, "C")) {
                replace(ft, "C", "");
            }
            if (isArray) {
                ft = ft + "[]";
            }
            ft = substitute(substitute(ft, "String", "string"), "Topic", "topic");
            replace(ft, "bool", "boolean");

            bool isOptional = field.is_flags & IS_OMITEMPTY;
            if (field.name == "fromName" || field.name == "toName") {
                isOptional = true;
            }
            fieldStream << "  " << field.name << (isOptional ? "?" : "") << ": " << ft << endl;
            replace(ft, "[]", "");

            if (ft != modelName) {
                imports[type2Key(toLower(ft))] = ft;
            }
        }

        const char* STR_HEADER1 =
            "/* eslint object-curly-newline: [\"error\", \"never\"] */\n"
            "/* eslint max-len: [\"error\", 160] */\n"
            "/*\n"
            " * This file was generated with makeClass --sdk. Do not edit it.\n"
            " */[{IMPORTS}]\n";

        ostringstream os;
        os << STR_HEADER1;
        os << "export type " << modelName << " = {" << endl;
        os << fieldStream.str();
        os << "}" << endl;
        string_q contents = os.str();

        if (imports.size() > 0) {
            ostringstream imps;
            for (auto imp : imports) {
                if (imp.second == "string" || imp.second == "boolean") {
                    continue;
                }
                if (imps.str() != "") {
                    imps << ", ";
                }
                imps << imp.second;
            }

            if (imps.str() != "") {
                ostringstream importLine;
                importLine << endl << "import { " << imps.str() << " } from '.';" << endl;
                replace(contents, "[{IMPORTS}]", importLine.str());
            } else {
                replace(contents, "[{IMPORTS}]", "");
            }
        }

        typesOut.push_back(modelName);
        string_q fileName = sdkPath + "typescript/src/types/" + firstLower(modelName) + ".ts";
        writeIfDifferent(fileName, contents);
    }

    return true;
}

//------------------------------------------------------------------------------------------------------------
bool COptions::handle_sdk_ts_paths(CStringArray& pathsOut) {
    for (auto ep : endpointArray) {
        CCommandOptionArray params;
        for (auto option : routeOptionArray) {
            if (option.api_route == ep.api_route && isChifraRoute(option, false)) {
                params.push_back(option);
            }
        }
        ep.params = &params;

        if (!ep.api_route.empty()) {
            map<string_q, string_q> imports;
            ostringstream rets;
            CStringArray types;
            getReturnTypes(ep, types);
            if (types.empty()) {
                // LOG_WARN("Skipping " + ep.api_route + " because it has no return types.");
                continue;
            }

            ostringstream filename;
            filename << sdkPath << "typescript/src/paths/" << ep.api_route << ".ts";

            for (auto type : types) {
                if (rets.str() != "" && rets.str() != "void") {
                    rets << " | ";
                }
                rets << firstUpper(type) << "[]";
                imports[type] = firstUpper(type);
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

            const char* STR_HEADER2 =
                "/* eslint object-curly-newline: [\"error\", \"never\"] */\n"
                "/* eslint max-len: [\"error\", 160] */\n"
                "/*\n"
                " * This file was generated with makeClass --sdk. Do not edit it.\n"
                " */\n";

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
                const char* STR_TOOL_NOT_AVAIL =
                    "//\n"
                    "// The [{ROUTE}] tool is not available in the SDK.\n"
                    "//\n"
                    "export default function () { } // no semicolon!\n";
                out += substitute(STR_TOOL_NOT_AVAIL, "[{ROUTE}]", ep.api_route);
            }

            pathsOut.push_back(ep.api_route);
            writeIfDifferent(filename.str(), out);
        }

        counter.cmdCount += params.size();
        counter.routeCount++;
    }

    LOG_INFO(cYellow, "makeClass --sdk (typescript)", cOff, " processed ", counter.routeCount, "/", counter.cmdCount,
             " paths ", " (changed ", counter.nProcessed, ").", string_q(40, ' '));

    return true;
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
string_q type2Key(const string_q& in) {
    string_q ret = in;
    if (contains(ret, "list<")) {
        ret = substitute(ret, "list<", "");
        ret = substitute(ret, ">", "");
    }
    if (contains(ret, "enum[")) {
        ret = substitute(ret, "enum[", "");
        ret = substitute(ret, "]", "");
    }
    if (startsWith(ret, "int")) {
        string_q key = substitute(ret, "int", "");
        ret = "int" + padLeft(key, 5, '0');
    }
    if (startsWith(ret, "uint")) {
        string_q key = substitute(ret, "uint", "");
        ret = "uint" + padLeft(key, 5, '0');
    }
    ret = toLower(ret);
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
    "  },\n"
    "  options?: RequestInit,\n"
    ") {\n"
    "  return ApiCallers.fetch<[{RETTYPE}]>(\n"
    "    { endpoint: '/[{LOWER}]', method: 'get', parameters, options },\n"
    "  );\n"
    "}\n";
