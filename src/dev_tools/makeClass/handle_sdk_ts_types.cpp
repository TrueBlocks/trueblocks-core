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
bool COptions::handle_sdk_ts_types(CStringArray& typesOut) {
    for (auto model : dataModels) {
        if (model.doc_route == "no_doc") {
            continue;
        }
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
                field.type == "topic" || startsWith(field.name, "unused") || contains(field.name, "::") ||
                containsI(field.type, "storageslot") || field.type % "any") {
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
            ft = substitute(ft, "String", "string");
            ft = substitute(ft, "Topic", "topic");
            ft = substitute(ft, "Address", "address");
            ft = substitute(ft, "Any", "any");
            ft = substitute(ft, "Hash", "hash");
            ft = substitute(ft, "[]string", "string[]");
            replace(ft, "bool", "boolean");

            bool isOptional = field.memberFlags & IS_OMITEMPTY;
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
                if (imp.second == "string" || imp.second == "boolean" || imp.second == "any") {
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
