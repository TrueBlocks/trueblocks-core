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

extern const char* STR_YAML_TAIL;
extern const char* STR_DOCUMENT_TAIL;
extern const char* STR_YAML_MODELHEADER;
extern const char* STR_MODEL_PRODUCERS;
extern const char* STR_MODEL_FOOTER;
extern const char* STR_MODEL_HEADER;
extern void generate_go_type(COptions* opts, const CClassDefinition& model);
extern void addToTypeMap(map<string_q, string_q>& map, const string_q& group, const string& type);
extern bool sortByDataModelName(const CClassDefinition& c1, const CClassDefinition& c2);
extern bool sortByDoc(const CMember& c1, const CMember& c2);
extern string_q typeFmt(const CMember& fld);
extern string_q get_producer_table(const CClassDefinition& model, const CCommandOptionArray& endpoints);
extern string_q type_2_Link(const CClassDefinitionArray& dataModels, const CMember& member);
extern string_q plural(const string_q& in);

//------------------------------------------------------------------------------------------------------------
bool COptions::handle_datamodel(void) {
    sort(dataModels.begin(), dataModels.end(), sortByDataModelName);

    uint32_t weight = 1000;

    map<string_q, string_q> documentMap;
    map<string_q, string_q> typeMaps;
    map<string_q, bool> frontMatterMap;
    CNameValueMap baseTypes;
    asciiFileToMap(getDocsPathTemplates("base-types.csv"), baseTypes);

    ostringstream yamlStream;
    yamlStream << "components:" << endl;
    yamlStream << "  schemas:" << endl;

    bool badStuff = false;
    for (auto model : dataModels) {
        if (model.doc_route == "no_doc") {
            continue;
        }
        string_q groupLow = toLower(substitute(model.doc_group, " ", ""));
        string_q groupFn = getDocsPathTemplates("model-groups/" + groupLow + ".md");
        if (!fileExists(groupFn)) {
            LOG_WARN("Missing data model intro file: ", bYellow, getPathToTemplates(groupFn), cOff);
            badStuff = true;
            continue;
        }

        sort(model.fieldArray.begin(), model.fieldArray.end(), sortByDoc);

        size_t fieldWidths[5];
        bzero(fieldWidths, sizeof(fieldWidths));
        for (auto& fld : model.fieldArray) {
            if (fld.doc) {
                replaceAll(fld.description, "&#44;", ",");
                fieldWidths[0] = max(size_t(3), max(fieldWidths[0], fld.name.length()));
                fieldWidths[1] = max(size_t(3), max(fieldWidths[1], fld.description.length()));
                fieldWidths[2] = max(size_t(3), max(fieldWidths[2], type_2_Link(dataModels, fld).length()));
            }
        }

        ostringstream docStream;
        if (!frontMatterMap[model.doc_group]) {
            frontMatterMap[model.doc_group] = true;
            string_q front = STR_YAML_FRONTMATTER;
            replace(front, "[{TITLE}]", firstUpper(toLower(model.doc_group)));
            replace(front, "[{WEIGHT}]", uint_2_Str(model.doc_group == "Admin" ? 1700 : weight));
            replace(front, "[{M1}]", "data:");
            replace(front, "[{M2}]", "parent: \"collections\"");
            replace(front, "[{HUGO_ALIASES}]", getAliases(this, "data-model", model.doc_group));
            docStream << front << endl;
            docStream << asciiFileToString(groupFn);
            weight += 200;
        }

        string_q modelFn = getDocsPathTemplates("model-intros/" + model.doc_route + ".md");
        if (!fileExists(modelFn)) {
            LOG_WARN("Missing data model intro file: ", bYellow, getPathToTemplates(modelFn), cOff);
            badStuff = true;
            continue;
        } else {
            docStream << STR_MODEL_HEADER << asciiFileToString(modelFn) << get_producer_table(model, endpointArray)
                      << STR_MODEL_FOOTER << endl;
            string_q notesFn = getDocsPathTemplates("model-intros/" + model.doc_route + ".notes.md");
            if (fileExists(notesFn)) {
                docStream << "### Notes\n\n" << asciiFileToString(notesFn) << endl;
            }
        }

        ostringstream fieldStream, toolsStream;
        fieldStream << markDownRow("Field", "Description", "Type", fieldWidths);
        fieldStream << markDownRow("-", "", "", fieldWidths);

        size_t cnt = 0;
        ostringstream yamlPropStream;
        for (auto fld : model.fieldArray) {
            if (fld.doc) {
                yamlPropStream << fld.Format("[        {NAME}:\n]");
                yamlPropStream << fld.Format(typeFmt(fld));
                yamlPropStream << fld.Format("[          description: \"{DESCRIPTION}\"\n]");
                fieldStream << markDownRow(fld.name, fld.description, type_2_Link(dataModels, fld), fieldWidths);
                addToTypeMap(typeMaps, model.doc_group, fld.type);
                cnt++;
            }
        }
        if (cnt == 0) {
            cerr << bRed << "Data model for " << model.class_name << " has zero documented fields." << cOff << endl;
            exit(0);
        }

        string_q head = model.Format(STR_YAML_MODELHEADER);
        yamlStream << head;
        yamlStream << yamlPropStream.str();

        string_q thisDoc = docStream.str();
        replaceAll(thisDoc, "[{TYPE}]", model.doc_route);
        replaceAll(thisDoc, "[{FIRST_UPPER}]", substitute(firstUpper(model.doc_route), "Config", "Status"));
        replaceAll(thisDoc, "[{PLURAL}]", plural(model.doc_alias.empty() ? model.doc_route : model.doc_alias));
        replaceAll(thisDoc, "[{PROPER}]", toProper(model.doc_route));
        if (contains(thisDoc, "[{FIELDS}]"))
            replace(thisDoc, "[{FIELDS}]", trim(fieldStream.str(), '\n'));
        else
            thisDoc += fieldStream.str();
        if (contains(thisDoc, "[{TOOLS}]"))
            replace(thisDoc, "[{TOOLS}]", trim(toolsStream.str(), '\n'));
        else
            thisDoc += toolsStream.str();
        documentMap[model.doc_group] = documentMap[model.doc_group] + thisDoc;

        if (!model.go_model.empty()) {
            generate_go_type(this, model);
        }
    }
    if (badStuff) {
        exit(0);
    }

    yamlStream << STR_YAML_TAIL;
    string_q doc = substitute(yamlStream.str(), "&#44;", ",");
    writeIfDifferent(getDocsPathTemplates("api/components.txt"), doc);

    for (auto document : documentMap) {
        ostringstream tailStream;
        CStringArray docTypes;
        explode(docTypes, typeMaps[toLower(document.first)], ',');
        sort(docTypes.begin(), docTypes.end());

        size_t wids[5];
        bzero(wids, sizeof(size_t) * 5);
        wids[0] = max(size_t(3), max(wids[0], string_q("Type").length()));
        wids[1] = max(size_t(3), max(wids[1], string_q("Description").length()));
        wids[2] = max(size_t(3), max(wids[2], string_q("Notes").length()));
        for (auto type : docTypes) {
            string_q notes = baseTypes[type];
            string_q descr = substitute(nextTokenClear(notes, ','), "&#44;", ",");
            wids[0] = max(size_t(3), max(wids[0], type.length()));
            wids[1] = max(size_t(3), max(wids[1], descr.length()));
            wids[2] = max(size_t(3), max(wids[2], notes.length()));
        }

        tailStream << markDownRow("Type", "Description", "Notes", wids);
        tailStream << markDownRow("-", "", "", wids);
        for (auto type : docTypes) {
            string_q notes = baseTypes[type];
            string_q descr = substitute(nextTokenClear(notes, ','), "&#44;", ",");
            tailStream << markDownRow(type, descr, notes, wids);
        }

        document.second += substitute(STR_DOCUMENT_TAIL, "[{TYPES}]", tailStream.str());
        string_q outFn = getDocsPathContent("data-model/" + substitute(toLower(document.first), " ", "")) + ".md";
        if (!contains(outFn, "/.md")) {
            writeIfDifferent(outFn, substitute(document.second, "\n\n\n", "\n\n"));
        }
    }

    return true;
}

//------------------------------------------------------------------------------------------------------------
bool sortByDataModelName(const CClassDefinition& c1, const CClassDefinition& c2) {
    if (c1.doc_order == c2.doc_order) {
        return c1.class_name < c2.class_name;
    }
    return c1.doc_order < c2.doc_order;
}

//------------------------------------------------------------------------------------------------------------
bool sortByDoc(const CMember& c1, const CMember& c2) {
    if (c1.doc == c2.doc) {
        return c1.name < c2.name;
    }
    return c1.doc < c2.doc;
}

//------------------------------------------------------------------------------------------------------------
string_q type_2_ModelName(const string_q& type, bool raw) {
    string_q ret = type;
    replace(ret, "Array", "");
    return raw ? nextTokenClear(ret, '[') : ret;
}

//------------------------------------------------------------------------------------------------------------
string_q type_2_TypeName(const string_q& type, bool raw) {
    string_q ret = type;
    if (startsWith(ret, "C"))
        replace(ret, "C", "");
    replace(ret, "Array", "");
    return raw ? nextTokenClear(ret, '[') : ret;
}

//------------------------------------------------------------------------------------------------------------
string_q typeFmt(const CMember& fld) {
    if (fld.memberFlags & IS_ARRAY) {
        string_q ret = "          type: array\n          items:\n            $ref: \"#/components/schemas/++X++\"\n";
        string_q t = firstLower(type_2_TypeName(fld.type, false));
        if (contains(t, "any")) {
            t = "cacheItem";
        }
        replace(ret, "++X++", t);
        return ret;
    }

    if (fld.memberFlags & IS_OBJECT) {
        string_q ret = "          type: object\n          items:\n            $ref: \"#/components/schemas/++X++\"\n";
        replace(ret, "++X++", firstLower(type_2_TypeName(fld.type, false)));
        return ret;
    }

    if (fld.type == "[]string") {
        return "          type: array\n          items:\n            type: string\n";
    }

    if (fld.type == "blknum" || fld.type == "uint64" || fld.type == "timestamp" || fld.type == "double" ||
        fld.type == "uint32") {
        return "[          type: number\n          format: {TYPE}\n]";
    }

    if (fld.type == "address" || fld.type == "ipfshash" || fld.type == "hash" || fld.type == "bytes" ||
        fld.type == "gas" || fld.type == "wei" || fld.type == "int256" || fld.type == "uint256" || fld.type == "date" ||
        fld.type == "blkrange" || fld.type == "datetime") {
        return "[          type: string\n          format: {TYPE}\n]";
    }

    if (fld.type == "bool" || fld.type == "uint8") {
        return "[          type: boolean\n]";
    }

    return "[          type: {TYPE}\n]";
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
string_q get_producer_group(const string_q& p, const CCommandOptionArray& endpoints) {
    for (auto ep : endpoints) {
        if (ep.api_route == p) {
            return substitute(toLower(ep.group), " ", "");
        }
    }
    return "unknown";
}

//------------------------------------------------------------------------------------------------------------
string_q get_producer_table(const CClassDefinition& model, const CCommandOptionArray& endpoints) {
    CStringArray producers;
    explode(producers, substitute(model.doc_producer, " ", ""), ',');
    ostringstream prodStream;
    for (auto p : producers) {
        string_q g = get_producer_group(p, endpoints);
        prodStream << "- [chifra " << p << "](/chifra/" << g << "/#chifra-" << p << ")" << endl;
    }
    return STR_MODEL_PRODUCERS + prodStream.str();
}

//------------------------------------------------------------------------------------------------------------
string_q findGroup(const CClassDefinitionArray& dataModels, const string_q& type) {
    for (auto model : dataModels) {
        if (model.base_lower == toLower(type)) {
            return model.doc_group;
        }
    }
    return "";
}

//------------------------------------------------------------------------------------------------------------
string_q type_2_Link(const CClassDefinitionArray& dataModels, const CMember& member) {
    string_q type = member.type;
    if (!startsWith(type, "C")) {
        return type;
    } else if (type == "CStringArray") {
        return "string[]";
    } else if (type == "CTopicArray") {
        return "topic[]";
    } else if (type == "CAddressArray") {
        return "address[]";
    } else if (type == "Cuint64Array") {
        return "uint64[]";
    }

    bool isArray = contains(type, "Array");
    replace(type, "C", "");
    replace(type, "Array", "");
    replace(type, "Ptr", "");

    string_q group = findGroup(dataModels, type);
    if (group.empty()) {
        return member.type;
    }

    return "[" + type + (isArray ? "[]" : "") + "](/data-model/" + toLower(substitute(group, " ", "")) + "/#" +
           toLower(type) + ")";
}

//--------------------------------------------------------------------------------
string_q plural(const string_q& in) {
    string_q ret = firstUpper(in);
    if (ret == "Status") {
        ret = "Statuses";
    } else if (ret == "CacheItem") {
        ret = "CacheItems";
    } else if (ret == "ChunkIndex") {
        ret = "ChunkIndexes";
    } else if (!endsWith(in, "s")) {
        ret += "s";
    }
    return ret;
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
    "    address:\n"
    "      type: string\n"
    "    string:\n"
    "      type: string\n"
    "    uint64:\n"
    "      type: number\n"
    "      format: uint64\n"
    "    topic:\n"
    "      type: string\n"
    "      format: bytes\n"
    "      description: \"One of four 32-byte topics of a log\"\n"
    "      example: \"0xf128...1e98\"\n"
    "\n";

//------------------------------------------------------------------------------------------------------------
const char* STR_DOCUMENT_TAIL =
    "## Base types\n"
    "\n"
    "This documentation mentions the following basic data types.\n"
    "\n"
    "[{TYPES}]";

//------------------------------------------------------------------------------------------------------------
const char* STR_YAML_MODELHEADER =
    "[    {DOC_ROUTE}:\n]"
    "[      description: \"{DOC_DESCR}\"\n]"
    "[      type: object\n]"
    "[      properties:\n]";

//------------------------------------------------------------------------------------------------------------
const char* STR_MODEL_FOOTER =
    "\n"
    "[{PLURAL}] consist of the following fields:\n"
    "\n"
    "[{FIELDS}]\n";

//------------------------------------------------------------------------------------------------------------
const char* STR_MODEL_PRODUCERS =
    "\n"
    "The following commands produce and manage [{PLURAL}]:\n"
    "\n";

//------------------------------------------------------------------------------------------------------------
const char* STR_MODEL_HEADER =
    "\n"
    "## [{FIRST_UPPER}]\n"
    "\n";
