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

extern const char* STR_TAIL_THING;
extern bool sortByDataModelName(const CClassDefinition& c1, const CClassDefinition& c2);
extern bool sortByDoc(const CParameter& c1, const CParameter& c2);
extern string_q typeFmt(const CParameter& fld);
extern string_q exFmt(const CParameter& fld);

//------------------------------------------------------------------------------------------------------------
bool COptions::handle_datamodel(void) {
    CClassDefinitionArray dataModels;

    for (auto classDefIn : classDefs) {
        CToml toml(classDefIn.input_path);
        CClassDefinition classDef(toml);
        classDef.short_fn = classDefIn.short_fn;
        classDef.input_path = classDefIn.input_path;
        if (!classDef.openapi.empty())
            dataModels.push_back(classDef);
    }

    sort(dataModels.begin(), dataModels.end(), sortByDataModelName);

    ostringstream theStream;
    theStream << "components:" << endl;
    theStream << "  schemas:" << endl;
    for (auto model : dataModels) {
        if (!model.openapi.empty()) {
            sort(model.fieldArray.begin(), model.fieldArray.end(), sortByDoc);
            string_q fmt;
            fmt += "[    {OPENAPI}:\n]";
            fmt += "[      description: \"{DESCRIPTION}\"\n]";
            fmt += "[      type: object\n]";
            fmt += "[      properties:\n]";
            theStream << model.Format(fmt);
            ostringstream props;
            for (auto fld : model.fieldArray) {
                if (fld.doc) {
                    props << fld.Format("[        {NAME}:\n]");
                    props << fld.Format(typeFmt(fld));
                    props << fld.Format(exFmt(fld));
                    props << fld.Format("[          description: \"{DESCRIPTION}\"\n]");
                }
            }
            theStream << props.str();
        }
    }
    theStream << STR_TAIL_THING;
    stringToAsciiFile(getDocsTemplate("api/templates/components.txt"), substitute(theStream.str(), "&#44;", ","));

    return true;
}

//------------------------------------------------------------------------------------------------------------
bool sortByDataModelName(const CClassDefinition& c1, const CClassDefinition& c2) {
    return c1.openapi < c2.openapi;
}

//------------------------------------------------------------------------------------------------------------
bool sortByDoc(const CParameter& c1, const CParameter& c2) {
    return c1.doc < c2.doc;
}

//------------------------------------------------------------------------------------------------------------
string_q typeFmt(const CParameter& fld) {
    if (fld.is_flags & IS_ARRAY) {
        string_q ret = "          type: array\n          items:\n            $ref: \"#/components/schemas/++X++\"\n";
        string_q t = fld.type;
        if (startsWith(t, "C"))
            replace(t, "C", "");
        replace(t, "Array", "");
        replace(ret, "++X++", string_q(1, (char)tolower(t[0])) + t.substr(1, 100));
        replace(ret, "logEntry", "log");
        return ret;
    }
    if (fld.is_flags & IS_OBJECT) {
        string_q ret = "          type: object\n          items:\n            $ref: \"#/components/schemas/++X++\"\n";
        string_q t = fld.type;
        if (startsWith(t, "C"))
            replace(t, "C", "");
        replace(t, "Array", "");
        replace(ret, "++X++", string_q(1, (char)tolower(t[0])) + t.substr(1, 100));
        replace(ret, "logEntry", "log");
        return ret;
    }
    if (fld.type == "blknum" || fld.type == "uint64" || fld.type == "timestamp" || fld.type == "double" ||
        fld.type == "uint32")
        return "[          type: number\n          format: {TYPE}\n]";
    if (fld.type == "address" || fld.type == "hash" || fld.type == "bytes" || fld.type == "gas" || fld.type == "wei" ||
        fld.type == "int256" || fld.type == "uint256" || fld.type == "date")
        return "[          type: string\n          format: {TYPE}\n]";
    if (fld.type == "bool" || fld.type == "uint8")
        return "[          type: boolean\n]";
    return "[          type: {TYPE}\n]";
}

//------------------------------------------------------------------------------------------------------------
string_q exFmt(const CParameter& fld) {
    if (fld.type == "blknum" || fld.type == "uint64" || fld.type == "timestamp" || fld.type == "bool" ||
        fld.type == "double")
        return "[          example: {EXAMPLE}\n]";
    return "[          example: \"{EXAMPLE}\"\n]";
}

//------------------------------------------------------------------------------------------------------------
const char* STR_TAIL_THING =
    "    response:\n"
    "      required:\n"
    "        - result\n"
    "      type: object\n"
    "      properties:\n"
    "        data:\n"
    "          type: object\n"
    "        error:\n"
    "          type: array\n"
    "          example:\n"
    "            - error 1\n"
    "            - error 2\n"
    "          items:\n"
    "            type: string\n"
    "    hash:\n"
    "      type: string\n"
    "      format: hash\n"
    "      description: \"The 32-byte hash\"\n"
    "      example: \"0xf128...1e98\"\n"
    "    string:\n"
    "      type: string\n"
    "    topic:\n"
    "      type: string\n"
    "      format: bytes\n"
    "      description: \"One of four 32-byte topics of a log\"\n"
    "      example: \"0xf128...1e98\"\n"
    "\n";
