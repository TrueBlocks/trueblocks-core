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
        size_t widths[5];
        bzero(widths, sizeof(widths));
        for (auto& fld : model.fieldArray) {
            if (fld.doc) {
                replaceAll(fld.description, "&#44;", ",");
                widths[0] = max(size_t(3), max(widths[0], fld.name.length()));
                widths[1] = max(size_t(3), max(widths[1], fld.description.length()));
                widths[2] = max(size_t(3), max(widths[2], fld.type.length()));
            }
        }
        string_q fmt;
        ostringstream doc;
        if (!dataDocsFront[model.doc_group]) {
            dataDocsFront[model.doc_group] = true;
            string_q front = STR_YAML_FRONTMATTER;
            replace(front, "[{TITLE}]", model.doc_group);
            replace(front, "[{WEIGHT}]", uint_2_Str(model.doc_group == "Admin" ? 1700 : weight));
            replace(front, "[{M1}]", "data:");
            replace(front, "[{M2}]", "parent: \"collections\"");
            doc << front << endl;
            weight += 200;
            doc << asciiFileToString(
                getDocsPathTemplates("model-groups/" + substitute(toLower(model.doc_group), " ", "") + ".md"));
        }

        string_q name = model.doc_api;
        if (name.length())
            name[0] = (char)toupper(name[0]);
        doc << endl;
        doc << "## " << name << endl;
        doc << endl;
        doc << asciiFileToString(getDocsPathTemplates("model-intros/" + model.doc_api) + ".md") << endl;

        doc << "### Fields" << endl;
        doc << endl;
        doc << markDownRow("Field", "Description", "Type", widths);
        doc << markDownRow("-", "", "", widths);

        fmt += "[    {DOC_API}:\n]";
        fmt += "[      description: \"{DOC_DESCR}\"\n]";
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
                doc << markDownRow(fld.name, fld.description, fld.type, widths);
            }
        }
        theStream << props.str();
        dataDocs[model.doc_group] = dataDocs[model.doc_group] + doc.str();
    }
    theStream << STR_YAML_TAIL;
    stringToAsciiFile(getDocsPathTemplates("api/components.txt"), substitute(theStream.str(), "&#44;", ","));

    for (auto doc : dataDocs) {
        doc.second += STR_YAML_TAIL2;
        string_q outFn = getDocsPathContent("data-model/" + substitute(toLower(doc.first), " ", "")) + ".md";
        writeIfDifferent(outFn, doc.second, Now());
    }

    return true;
}

//------------------------------------------------------------------------------------------------------------
bool sortByDataModelName(const CClassDefinition& c1, const CClassDefinition& c2) {
    return c1.doc_order < c2.doc_order;
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
    "\n"
    "## Base types\n"
    "\n"
    "In these docs, sometimes Trueblocks mentions a type format that is more\n"
    "precise than the generic types, like \"string\" or \"object\".\n"
    "\n"
    "| Type      | Description                                     | Notes          |\n"
    "| --------- | ----------------------------------------------- | -------------- |\n"
    "| address   | a 20-byte hexidecimal string starting with '0x' | lowercase      |\n"
    "| blknum    | an alias for a uint64                           |                |\n"
    "| bool      | a value either `true`, `false`, `1`, or `0`     |                |\n"
    "| bytes     | an arbitrarily long string of bytes             |                |\n"
    "| date      | a JSON formatted date                           | as a string    |\n"
    "| double    | a floating point number of double precision     |                |\n"
    "| gas       | an unsigned big number                          | as a string    |\n"
    "| hash      | a 32-byte hexidecimal string starting with '0x' | lowercase      |\n"
    "| int256    | a signed big number                             | as a string    |\n"
    "| ipfshash  | a multi-hash produced by IPFS                   | mixed-case     |\n"
    "| string    | a normal character string                       |                |\n"
    "| timestamp | a 64-bit unsigned integer                       | unix timestamp |\n"
    "| uint32    | a 32-bit unsigned integer                       |                |\n"
    "| uint64    | a 64-bit unsigned integer                       |                |\n"
    "| uint8     | an alias for the boolean type                   |                |\n"
    "| wei       | an unsigned big number                          | as a string    |\n";
