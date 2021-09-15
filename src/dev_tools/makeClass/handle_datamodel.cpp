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

extern const char* STR_YAML_TAIL;
extern const char* STR_YAML_TAIL2;
extern bool sortByDataModelName(const CClassDefinition& c1, const CClassDefinition& c2);
extern bool sortByDoc(const CParameter& c1, const CParameter& c2);
extern string_q typeFmt(const CParameter& fld);
extern string_q exFmt(const CParameter& fld);

//------------------------------------------------------------------------------------------------------------
bool COptions::handle_datamodel(void) {
    sort(dataModels.begin(), dataModels.end(), sortByDataModelName);

    uint32_t weight = 1000;

    map<string_q, string_q> dataDocs;
    map<string_q, bool> dataDocsFront;
    ostringstream theStream;
    theStream << "components:" << endl;
    theStream << "  schemas:" << endl;
    for (auto model : dataModels) {
        sort(model.fieldArray.begin(), model.fieldArray.end(), sortByDoc);
        string_q fmt;
        ostringstream doc;
        if (!dataDocsFront[model.doc_group]) {
            dataDocsFront[model.doc_group] = true;
            string_q front = STR_YAML_FRONTMATTER;
            replace(front, "[{TITLE}]", model.doc_group);
            replace(front, "[{WEIGHT}]", uint_2_Str(weight));
            replace(front, "[{M1}]", "data:");
            replace(front, "[{M2}]", "parent: \"collections\"");
            doc << front << endl;
            weight += 200;
            doc << asciiFileToString(
                getDocsTemplate("model-groups/" + substitute(toLower(model.doc_group), " ", "") + ".md"));
        }

        string_q name = model.openapi;
        if (name.length())
            name[0] = (char)toupper(name[0]);
        doc << endl;
        doc << "## " << name << endl;
        doc << endl;
        doc << asciiFileToString(getDocsTemplate("model-intros/" + model.openapi) + ".md") << endl;

        doc << "### Fields" << endl;
        doc << endl;
        doc << "| Field | Description | Type |" << endl;
        doc << "|-------|-------------|------|" << endl;

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

                doc << "| " << fld.name << " | " << fld.description << " | " << fld.type << " |" << endl;
            }
        }
        theStream << props.str();
        dataDocs[model.doc_group] = dataDocs[model.doc_group] + doc.str();
    }
    theStream << STR_YAML_TAIL;
    stringToAsciiFile(getDocsTemplate("api/templates/components.txt"), substitute(theStream.str(), "&#44;", ","));

    for (auto doc : dataDocs) {
        doc.second += STR_YAML_TAIL2;
        string_q res = substitute(doc.second, "$DATE", "2021-06-30T12:13:03-03:00");
        stringToAsciiFile(getDataModelPath(substitute(toLower(doc.first), " ", "")) + ".md", res);
    }

    return true;
}

//------------------------------------------------------------------------------------------------------------
bool sortByDataModelName(const CClassDefinition& c1, const CClassDefinition& c2) {
    return c1.doc_group + c1.doc_order + c1.openapi < c2.doc_group + c2.doc_order + c2.openapi;
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
        if (t.length())
            t[0] = (char)tolower(t[0]);
        replace(ret, "++X++", t);
        replace(ret, "logEntry", "log");
        return ret;
    }
    if (fld.is_flags & IS_OBJECT) {
        string_q ret = "          type: object\n          items:\n            $ref: \"#/components/schemas/++X++\"\n";
        string_q t = fld.type;
        if (startsWith(t, "C"))
            replace(t, "C", "");
        replace(t, "Array", "");
        if (t.length())
            t[0] = (char)tolower(t[0]);
        replace(ret, "++X++", t);
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
const char* STR_YAML_TAIL =
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

//------------------------------------------------------------------------------------------------------------
const char* STR_YAML_TAIL2 =
    "## Base types\n"
    "\n"
    "In these docs, sometimes Trueblocks mentions a type format that is more\n"
    "precise than the generic types, like \"string\" or \"object\".\n"
    "\n"
    "| Type Name | Description                         |\n"
    "| --------- | ----------------------------------- |\n"
    "| blknum    | a 64-bit unsigned int               |\n"
    "| timestamp | a 64-bit unsigned int               |\n"
    "| address   | a 20 byte string starting with '0x' |\n"
    "| hash      | a 32 byte string starting with '0x' |\n"
    "| string    | a plain c++ string                  |\n"
    "| number    | standard c++ 64-bit unsigned int    |\n"
    "| bigint    | arbitrarily sized signed int        |\n"
    "| wei       | arbitrarily sized unsigned int      |\n"
    "| boolean   | standard c++ boolean                |\n";
