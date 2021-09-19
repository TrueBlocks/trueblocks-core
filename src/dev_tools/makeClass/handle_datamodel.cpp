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
extern const char* STR_DOCUMENT_TAIL;
extern const char* STR_YAML_MODELHEADER;
extern const char* STR_FIELDS_INTRO;
extern void addToTypeMap(map<string_q, string_q>& map, const string_q& group, const string& type);
extern bool sortByDataModelName(const CClassDefinition& c1, const CClassDefinition& c2);
extern bool sortByDoc(const CParameter& c1, const CParameter& c2);
extern string_q typeFmt(const CParameter& fld);
extern string_q exFmt(const CParameter& fld);

//------------------------------------------------------------------------------------------------------------
bool COptions::handle_datamodel(void) {
    sort(dataModels.begin(), dataModels.end(), sortByDataModelName);

    uint32_t weight = 1000;

    map<string_q, string_q> documentMap;
    map<string_q, string_q> typeMaps;
    map<string_q, bool> frontMatterMap;
    CNameValueMap types;
    asciiFileToMap(getDocsPathTemplates("base-types.csv"), types);

    ostringstream yamlStream;
    yamlStream << "components:" << endl;
    yamlStream << "  schemas:" << endl;

    for (auto model : dataModels) {
        string_q groupLow = toLower(substitute(model.doc_group, " ", ""));
        string_q groupFn = getDocsPathTemplates("model-groups/" + groupLow + ".md");
        string_q modelFn = getDocsPathTemplates("model-intros/" + model.doc_api + ".md");

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

        ostringstream docStream;
        if (!frontMatterMap[model.doc_group]) {
            frontMatterMap[model.doc_group] = true;
            string_q front = STR_YAML_FRONTMATTER;
            replace(front, "[{TITLE}]", model.doc_group);
            replace(front, "[{WEIGHT}]", uint_2_Str(model.doc_group == "Admin" ? 1700 : weight));
            replace(front, "[{M1}]", "data:");
            replace(front, "[{M2}]", "parent: \"collections\"");
            docStream << front << endl;
            docStream << asciiFileToString(groupFn);
            weight += 200;
        }

        docStream << endl;
        docStream << "## " << firstUpper(model.doc_api) << endl;
        docStream << endl;
        docStream << asciiFileToString(modelFn) << endl;

        string_q fieldIntro = STR_FIELDS_INTRO;
        replace(fieldIntro, "[{TYPE}]", model.doc_api);
        replaceAll(fieldIntro, "[{PLURAL}]", plural(model.doc_api, 0));

        ostringstream fieldStream;
        fieldStream << fieldIntro << endl;
        fieldStream << markDownRow("Field", "Description", "Type", widths);
        fieldStream << markDownRow("-", "", "", widths);

        ostringstream yamlPropStream;
        for (auto fld : model.fieldArray) {
            if (fld.doc) {
                yamlPropStream << fld.Format("[        {NAME}:\n]");
                yamlPropStream << fld.Format(typeFmt(fld));
                yamlPropStream << fld.Format(exFmt(fld));
                yamlPropStream << fld.Format("[          description: \"{DESCRIPTION}\"\n]");
                fieldStream << markDownRow(fld.name, fld.description, fld.type, widths);
                addToTypeMap(typeMaps, model.doc_group, fld.type);
            }
        }

        yamlStream << model.Format(STR_YAML_MODELHEADER);
        yamlStream << yamlPropStream.str();

        string_q thisDoc = docStream.str();
        if (contains(thisDoc, "[{FIELDS}]"))
            replace(thisDoc, "[{FIELDS}]", trim(fieldStream.str(), '\n'));
        else
            thisDoc += fieldStream.str();

        documentMap[model.doc_group] = documentMap[model.doc_group] + thisDoc + "\n---\n";
    }

    yamlStream << STR_YAML_TAIL;
    writeIfDifferent(getDocsPathTemplates("api/components.txt"), substitute(yamlStream.str(), "&#44;", ","));

    for (auto document : documentMap) {
        string_q tail;
        CStringArray docTypes;
        explode(docTypes, typeMaps[toLower(document.first)], ',');
        sort(docTypes.begin(), docTypes.end());
        size_t wids[5];
        bzero(wids, sizeof(size_t) * 5);
        wids[0] = 9;
        wids[1] = 47;
        wids[2] = 14;
        for (auto type : docTypes) {
            string_q notes = types[type];
            string_q descr = substitute(nextTokenClear(notes, ','), "&#44;", ",");
            tail += markDownRow(type, descr, notes, wids);
        }
        document.second += substitute(STR_DOCUMENT_TAIL, "[{TYPES}]", tail);
        string_q outFn = getDocsPathContent("data-model/" + substitute(toLower(document.first), " ", "")) + ".md";
        writeIfDifferent(outFn, document.second, Now());
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
        replace(ret, "++X++", firstLower(t));
        replace(ret, "logEntry", "log");
        return ret;
    }

    if (fld.is_flags & IS_OBJECT) {
        string_q ret = "          type: object\n          items:\n            $ref: \"#/components/schemas/++X++\"\n";
        string_q t = fld.type;
        if (startsWith(t, "C"))
            replace(t, "C", "");
        replace(t, "Array", "");
        replace(ret, "++X++", firstLower(t));
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
void addToTypeMap(map<string_q, string_q>& map, const string_q& group, const string& type) {
    string_q existing = map[toLower(group)];
    if (contains("," + existing + ",", "," + type + ","))  // exact match
        return;
    if (toLower(type) != type)  // not a base type
        return;
    if (existing.length() > 0)
        existing += ",";
    map[toLower(group)] = existing + type;
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
const char* STR_DOCUMENT_TAIL =
    "\n"
    "## Base types\n"
    "\n"
    "The above documentation mentions the following basic data types.\n"
    "\n"
    "| Type      | Description                                     | Notes          |\n"
    "| --------- | ----------------------------------------------- | -------------- |\n"
    "[{TYPES}]";

//------------------------------------------------------------------------------------------------------------
const char* STR_YAML_MODELHEADER =
    "[    {DOC_API}:\n]"
    "[      description: \"{DOC_DESCR}\"\n]"
    "[      type: object\n]"
    "[      properties:\n]";

//------------------------------------------------------------------------------------------------------------
const char* STR_FIELDS_INTRO =
    "Below is a list of the data fields for [{PLURAL}]. Following that are the "
    "commands that produce or manage [{PLURAL}].\n";
