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
/*
 * Parts of this file were generated with makeClass --options. Edit only those parts of
 * the code outside of the BEG_CODE/END_CODE sections
 */
#include "options.h"

//---------------------------------------------------------------------------------------------------
bool isApiRoute(const string_q& route) {
    if (route == "daemon" || route == "blaze")
        return false;
    return !route.empty();
}

//---------------------------------------------------------------------------------------------------
string_q toApiTag(const CCommandOption& cmd) {
    if ((isApiRoute(cmd.tool) && !contains(cmd.tool, "explore")) || !cmd.is_visible_docs)
        return "";
    const char* STR_TAG_YAML =
        "  - name: [{GROUP}]\n"
        "    description: [{DESCRIPTION}]\n";
    return cmd.Format(STR_TAG_YAML);
}

//---------------------------------------------------------------------------------------------------
string_q toGoCall(const CCommandOption& cmd) {
    if (!isApiRoute(cmd.api_route))
        return "";

    const char* STR_ONEROUTE =
        "// [{GOROUTEFUNC}] [{DESCRIPTION}]\n"
        "func [{GOROUTEFUNC}](w http.ResponseWriter, r *http.Request) {\n"
        "\tif err := [{API_ROUTE}]Pkg.Serve[{PROPER}](w, r); err != nil {\n"
        "\t\tRespondWithError(w, http.StatusInternalServerError, err)\n"
        "\t}\n"
        "}";

    ostringstream os;
    os << endl;
    os << cmd.Format(STR_ONEROUTE) << endl;
    return os.str();
}

//---------------------------------------------------------------------------------------------------
string_q toGoPackage(const CCommandOption& cmd) {
    if (!isApiRoute(cmd.api_route))
        return "";

    const char* STR_NEW_CHIFRA_ROUTE =
        "\t[{API_ROUTE}]Pkg \"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/[{API_ROUTE}]\"";

    ostringstream os;
    os << cmd.Format(STR_NEW_CHIFRA_ROUTE) << endl;
    return os.str();
}

//---------------------------------------------------------------------------------------------------
string_q toGoRoute(const CCommandOption& cmd) {
    if (!isApiRoute(cmd.api_route))
        return "";

    ostringstream out;
    out << cmd.Format("\tRoute{\"[{GOROUTEFUNC}]\", \"GET\", \"/[{API_ROUTE}]\", [{GOROUTEFUNC}]},") << endl;
    return out.str();
}

//---------------------------------------------------------------------------------------------------
string_q prepareDescr(const string_q& in) {
    if (in.length() < 75)
        return in;
    return ">\n            " + substitute(in, "\n         ", "");
}

//---------------------------------------------------------------------------------------------------
bool isCrud(const string_q& cmd) {
    return cmd == "create" || cmd == "delete" || cmd == "update" || cmd == "remove" || cmd == "undelete";
}

//---------------------------------------------------------------------------------------------------
bool visitEnumItem(string_q& item, void* data) {
    ostringstream* osp = (ostringstream*)data;
    if (isNumeral(item)) {
        *osp << substitute("~- \"[{VAL}]\"\n", "[{VAL}]", item);
    } else {
        *osp << substitute("~- [{VAL}]\n", "[{VAL}]", item);
    }
    return true;
}

//---------------------------------------------------------------------------------------------------
bool forEveryEnum(APPLYFUNC func, const string_q& enumStr, void* data) {
    string_q es = substitute(substitute(enumStr, "list<", ""), ">", "");
    string_q cleaned = substitute(substitute(substitute(es, "*", ""), "enum[", ""), "]", "");
    CStringArray items;
    explode(items, cleaned, '|');
    for (auto item : items) {
        if (func) {
            if (!(*func)(item, data))
                return false;
        }
    }
    return true;
}

//---------------------------------------------------------------------------------------------------
string_q getSchema(const string_q& data_type, const CCommandOption* cmd = NULL) {
    string_q lead = "            ";

    if (contains(data_type, "list") && contains(data_type, "enum")) {
        ostringstream os;
        forEveryEnum(visitEnumItem, data_type, &os);
        string_q str_array_enum =
            "~type: array\n"
            "~items:\n"
            "~  type: string\n"
            "~  enum:\n" +
            substitute(substitute(trim(os.str(), '\n'), "~", "+    "), "+", "~");
        return substitute(str_array_enum, "~", lead);
    }

    if (contains(data_type, "list")) {
        string_q type = substitute(substitute(data_type, "list<", ""), ">", "");
        replace(type, "addr", "address_t");
        if (endsWith(type, "_t"))
            replaceReverse(type, "_t", "");
        string_q ret;
        ret += lead + "type: array\n";
        ret += lead + "items:\n";
        ret += lead + "  type: string\n";
        ret += lead + "  format: " + type;
        return ret;
    }

    if (contains(data_type, "boolean")) {
        string_q ret;
        ret += lead + "type: boolean";
        return ret;
    }

    if (contains(data_type, "uint") || contains(data_type, "double") || contains(data_type, "blknum")) {
        string_q ret;
        ret += lead + "type: number\n";
        ret += lead + "format: " + substitute(substitute(data_type, ">", ""), "<", "");
        return ret;
    }

    if (contains(data_type, "datetime")) {
        string_q ret;
        ret += lead + "type: " + "string\n";
        ret += lead + "format: date";
        return ret;
    }

    if (cmd && contains(data_type, "enum")) {
        ostringstream os;
        forEveryEnum(visitEnumItem, cmd->data_type, &os);

        string_q ret;
        ret += lead + "type: string\n";
        ret += lead + "enum:\n";
        ret += substitute(substitute(trim(os.str(), '\n'), "~", "+  "), "+", "~");
        return substitute(ret, "~", lead);
    }

    return lead + "type: " + "string";
}

//------------------------------------------------------------------------------------------------------------
CStringArray getGlobalsArray() {
    CStringArray globals;
    globals.push_back("chain:string");
    globals.push_back("noHeader:boolean");
    globals.push_back("fmt:string");
    globals.push_back("verbose:boolean");
    globals.push_back("ether:boolean");
    globals.push_back("raw:boolean");
    globals.push_back("cache:boolean");
    globals.push_back("decache:boolean");
    return globals;
}

//---------------------------------------------------------------------------------------------------
string_q getGlobalFeature(const string_q& route, const string_q& feature) {
    if (feature == "raw") {
        return "raw|report raw data directly from the source|boolean|-w";
    } else if (feature == "cache") {
        return "cache|force the results of the query into the cache|boolean|-o";
    } else if (feature == "decache") {
        return "decache|removes related items from the cache|boolean|-D";
    } else if (feature == "ether") {
        return "ether|export values in ether|boolean|-H";
    } else if (feature == "fmt") {
        return "fmt|export format, one of [none|json*|txt|csv]|string|-x";
    } else if (feature % "noheader") {
        return "noHeader|suppress the header in the output|boolean|-";
    } else if (feature % "chain") {
        return "chain|the chain to use|string|-";
    }
    return "";
}

//---------------------------------------------------------------------------------------------------
string_q toApiPath(const CCommandOption& cmd, const string_q& returnTypesIn, const string_q& corresponds,
                   const string_q& exampleFn) {
    if (!isApiRoute(cmd.api_route) || contains(cmd.api_route, "explore"))
        return "";

    //---------------------------------------------------------------------------------------------------
    const char* STR_PARAM_YAML =
        "        - name: [{NAME}]\n"
        "          description: [{DESCR}]\n"
        "          required: [{REQ}]\n"
        "          style: form\n"
        "          in: query\n"
        "          explode: true\n"
        "          schema:\n"
        "[{SCHEMA}]";

    string_q apiRoute = cmd.api_route;

    size_t fieldCnt = 0;
    bool hasDelete = false;
    ostringstream memberStream;
    for (auto member : *(CCommandOptionArray*)cmd.members) {
        bool needsTwoAddrs = containsI(member.description, "two or more addresses");
        hasDelete |= contains(member.longName, "delete");
        if (member.longName.empty() || !member.is_visible_docs)
            continue;
        if (!isCrud(member.longName)) {
            string_q optionName = toCamelCase(member.longName);
            string_q yp = STR_PARAM_YAML;
            replace(yp, "[{NAME}]", optionName);
            replace(yp, "[{DESCR}]", prepareDescr(member.swagger_descr));
            replace(yp, "[{REQ}]", member.is_required ? "true" : "false");
            replace(yp, "[{SCHEMA}]", getSchema(member.data_type, &member));
            if (needsTwoAddrs) {
                replace(yp, "            type: array\n", "            type: array\n            minItems: 2\n");
            }
            if (memberStream.str().empty())
                memberStream << "      parameters:\n";
            memberStream << yp << endl;
            fieldCnt++;
            // if (!(member.option_type % "positional")) {
            //     reportOneOption(apiRoute, optionName, "api");
            // }
        }
    }

    string_q caps = get_corrected_caps(cmd.capabilities);
    CStringArray globals;
    explode(globals, caps, '|');
    for (auto global : globals) {
        string_q g = getGlobalFeature(apiRoute, global);
        CStringArray parts;
        explode(parts, g, '|');
        if (parts.size() < 3)
            continue;
        string_q optionName = toCamelCase(parts[0]);
        string_q yp = STR_PARAM_YAML;
        replace(yp, "[{NAME}]", optionName);
        replace(yp, "[{DESCR}]", prepareDescr(parts[1]));
        replace(yp, "[{REQ}]", "false");
        replace(yp, "[{SCHEMA}]", getSchema(parts[2]));
        memberStream << yp << endl;
        fieldCnt++;
        // reportOneOption(apiRoute, optionName, "api");
    }

    if (fieldCnt == 0) {
        cerr << bRed << "The " << apiRoute << " data model has zero documented fields." << cOff << endl;
        exit(0);
    }

    ostringstream example;
    if (fileExists(exampleFn)) {
        string_q content = trim(asciiFileToString(exampleFn), '\n');
        if (!content.empty()) {
            if (!contains(content, string_q(18, ' '))) {
                replaceAll(content, "\n", "\n" + string_q(18, ' '));
                content = string_q(18, ' ') + content;
            }
            example << string_q(16, ' ') << "example:" << endl << content << endl;
        }
    }

    string_q content;
    ostringstream properties;
    string_q descr = returnTypesIn;
    string_q returnTypes = nextTokenClear(descr, '|');
    const char* STR_PROPERTIES =
        "data:\n"
        "  description: [{DESCR}]\n"
        "  type: array\n"
        "  items:\n"
        "    [{RETTYPES}]";
    content = substitute(substitute(STR_PROPERTIES, "[{RETTYPES}]", returnTypes), "[{DESCR}]", descr);

    if (!content.empty()) {
        if (!contains(content, string_q(18, ' '))) {
            replaceAll(content, "\n", "\n" + string_q(18, ' '));
            content = string_q(18, ' ') + content;
        }
        properties << string_q(16, ' ') << "properties:" << endl << content << endl;

    } else {
        if (!fileExists(exampleFn))
            properties << string_q(16, ' ') << "items:\n                  $ref: \"#/components/schemas/response\"\n";
    }

    string_q grp = substitute(substitute(cmd.group, "ChainData", "Chain Data"), "ChainState", "Chain State");

    //---------------------------------------------------------------------------------------------------
    const char* STR_PATH_YAML =
        "  /[{PATH}]:\n"
        "    get:\n"
        "      tags:\n"
        "        - [{TAGS}]\n"
        "      summary: [{SUMMARY}]\n"
        "      description: [{DESCR}]\n"
        "      operationId: [{ID}]\n"
        "[{PARAMS}][{DELETE}]"
        "      responses:\n"
        "        \"200\":\n"
        "          description: returns the requested data\n"
        "          content:\n"
        "            application/json:\n"
        "              schema:\n"
        "[{PROPERTIES}][{EXAMPLE}]        \"400\":\n"
        "          description: bad input parameter\n";

    //---------------------------------------------------------------------------------------------------
    const char* STR_DELETE_OPTS =
        "        - name: delete\n"
        "          description: delete the item, but do not remove it\n"
        "          required: false\n"
        "          style: form\n"
        "          in: query\n"
        "          explode: true\n"
        "          schema:\n"
        "            type: boolean\n"
        "        - name: undelete\n"
        "          description: undelete a previously deleted item\n"
        "          required: false\n"
        "          style: form\n"
        "          in: query\n"
        "          explode: true\n"
        "          schema:\n"
        "            type: boolean\n"
        "        - name: remove\n"
        "          description: remove a previously deleted item\n"
        "          required: false\n"
        "          style: form\n"
        "          in: query\n"
        "          explode: true\n"
        "          schema:\n"
        "            type: boolean\n";

    string_q ret = STR_PATH_YAML;
    replaceAll(ret, "[{TAGS}]", grp);
    replaceAll(ret, "[{PROPERTIES}]", properties.str());
    replaceAll(ret, "[{EXAMPLE}]", example.str());
    replaceAll(ret, "[{PATH}]", apiRoute);
    replaceAll(ret, "[{PARAMS}]", memberStream.str());
    replaceAll(ret, "[{SUMMARY}]", cmd.summary);
    replaceAll(ret, "[{DESCR}]", cmd.description + corresponds);
    replaceAll(ret, "[{DELETE}]", hasDelete ? STR_DELETE_OPTS : "");
    replaceAll(ret, "[{ID}]", toLower(substitute(grp, " ", "") + "-" + apiRoute));
    return ret;
}

//---------------------------------------------------------------------------------------------------
bool COptions::writeOpenApiFile(void) {
    if (!openapi)
        return true;

    LOG_INFO(cYellow, "handling openapi file...", cOff);
    counter = CCounter();  // reset

    map<string_q, string_q> pkgs;
    for (auto ep : endpointArray) {
        CCommandOptionArray members;
        for (auto option : routeOptionArray)
            if (option.api_route == ep.api_route && option.isChifraRoute(false))
                members.push_back(option);
        ep.members = &members;

        CStringArray unused;
        string_q returnTypes = getReturnTypes(ep, unused);
        string_q exampleFn = getDocsPathTemplates("api/examples/" + ep.api_route + ".txt");

        apiTagStream << toApiTag(ep);
        goCallStream << toGoCall(ep);
        goRouteStream << toGoRoute(ep);
        apiPathStream << toApiPath(
            ep, returnTypes, get_corresponds_link(toLower(substitute(ep.group, " ", "")), ep.api_route), exampleFn);

        string_q pkg = toGoPackage(ep);
        string_q nick = nextTokenClear(pkg, ' ');
        pkgs[nick] = substitute(pkg, "\n", "");

        counter.cmdCount += members.size();
        counter.routeCount++;
    }
    for (auto pkg : pkgs)
        goPkgStream << pkg.first << " " << pkg.second << endl;

    writeCodeOut(this, getPathToSource("apps/chifra/internal/daemon/routes.go"));
    writeCodeOut(this, getDocsPathContent("api/openapi.yaml"));

    LOG_INFO(cYellow, "makeClass --openapi", cOff, " processed ", counter.routeCount, "/", counter.cmdCount,
             " routes/cmds ", " (changed ", counter.nProcessed, ").", string_q(40, ' '));

    return true;
}

//------------------------------------------------------------------------------------------------------------
string_q get_model_group(const CClassDefinitionArray& models, const string_q& route) {
    for (auto model : models) {
        if (contains(model.doc_producer, toLower(route)))
            return toLower(substitute(model.doc_group, " ", ""));
    }
    return "unknown";
}

void get_models(const CClassDefinitionArray& models, CStringArray& result, const string_q& route);
//---------------------------------------------------------------------------------------------------
string_q get_api_text(const CClassDefinitionArray& models, const CClassDefinition& model, const string_q& toolGroup,
                      const string_q& toolRoute) {
    string_q ret = firstUpper(model.doc_descr) + (endsWith(model.doc_descr, ".") ? "" : ".");

    CStringArray types;
    get_models(models, types, toolRoute);
    ostringstream os;
    for (auto type : types) {
        string_q group = nextTokenClear(type, '|');
        if (!os.str().empty()) {
            os << ", ";
        }
        os << "<a href=\""
           << "/data-model/" << group << "/#" << type << "\">" << firstUpper(type) << "</a>";
    }
    if (types.size() > 1) {
        ret = "Produces " + os.str() + " data.";
    } else {
        ret += " Produces " + os.str() + " data.";
    }
    if (contains(ret, "</a>, <a href")) {
        replaceReverse(ret, "</a>, <a href", "</a>, and/or <a href");
    }

    ret += get_corresponds_link(toolGroup, toolRoute);

    return ret;
}

extern string_q get_producer_group(const string_q& p, const CCommandOptionArray& endpoints);
//---------------------------------------------------------------------------------------------------
string_q COptions::getReturnTypes(const CCommandOption& ep, CStringArray& typesOut) {
    if (!isApiRoute(ep.api_route) || contains(ep.api_route, "explore")) {
        return "";
    }

    string_q descr;
    for (auto model : dataModels) {
        if (contains(model.doc_producer, ep.api_route)) {
            typesOut.push_back(model.doc_route);
            if (descr.empty()) {
                string_q group = get_producer_group(ep.api_route, endpointArray);
                string_q route = ep.api_route;
                descr = get_api_text(dataModels, model, group, route);
            }
        }
    }

    string_q prods;
    for (auto p : typesOut) {
        replace(p, "cachePtr", "cache");
        prods += "$ref: \"#/components/schemas/" + p + "\"\n";
    }

    if (typesOut.size() > 1) {
        prods = "oneOf:\n" + prods;
        replaceAll(prods, "$ref:", "      - $ref:");
    }

    return trim(prods, '\n') + "|" + descr;
}
