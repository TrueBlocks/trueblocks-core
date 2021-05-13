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

#define routeCount fileCount
#define cmdCount nVisited
#define nChanged nProcessed

extern const char* STR_TAG_ENTRY_YAML;
extern const char* STR_PATH_ENTRY_YAML;
extern const char* STR_PATH_PARAM_YAML;
extern const char* STR_HTML_CODE;
extern const char* STR_HTML_CODE_HEADER;
extern const char* STR_HTML_CODE_FOOTER;
extern string_q getTypeStr(const CCommandOption& opt, const string_q& lead);

typedef map<string, CCommandOption> CEndpointMap;
extern bool loadEndpoints(CEndpointMap& endpointMap, CCommandOptionArray& endpointArray);
extern string_q getGoRoute(const string_q& route, const string_q& cmd);
extern string_q getGoCall(const string_q& tool, const string_q& route, const string_q& cmd);
//---------------------------------------------------------------------------------------------------
void COptions::writeOpenApiFile(void) {
    if (!openapi)
        return;
    LOG_INFO(cYellow, "handling openapi file...", cOff);
    counter = CCounter();  // reset

    CCommands commands;
    options_2_Commands(commands);

    CEndpointMap endpointMap;
    CCommandOptionArray endpointArray;
    loadEndpoints(endpointMap, endpointArray);

    for (auto co : endpointArray) {
        if (co.api_route.empty()) {
            chifra_stream_a << "    // -- " << co.group << endl;
            chifra_stream_b << "    \"" << substitute(toUpper(co.group), " ", "") << "|\"" << endl;
        } else {
            chifra_stream_a << "    {\"" << co.api_route << "\", \"" << co.tool << "\"}," << endl;
            chifra_stream_b << "    \"  " << padRight(co.api_route, 20) << co.description << "|\"" << endl;
        }

        if (co.is_visible) {
            if (!contains(co.tool, " ")) {
                chifra_stream_pm << "    ";
                chifra_stream_pm << "make_pair(\"" << co.tool << "\", \"chifra " << co.api_route << "\")," << endl;
            } else {
                chifra_stream_pm << "    // " << co.api_route << endl;
            }
        } else {
            if (co.api_route.empty()) {
                chifra_stream_pm << "    // -- " << co.group << endl;
            } else {
                chifra_stream_pm << "    // " << co.api_route << endl;
            }
        }
    }

    ostringstream yamlTagStream;
    ostringstream yamlPathStream;
    for (auto ep : endpointMap) {
        string_q group = ep.second.group;
        string_q tool = ep.second.tool;
        // string_q command = ep.second.route_list;
        string_q description = ep.second.description;

        yamlTagStream << substitute(substitute(STR_TAG_ENTRY_YAML, "[{NAME}]", group), "[{DESCR}]", description);

        CStringArray cmds;
        explode(cmds, ep.second.route_list, ',');
        for (auto cmd : cmds) {
            string_q yamlEntry = STR_PATH_ENTRY_YAML;
            replace(yamlEntry, "[{TAGS}]", group);
            replace(yamlEntry, "[{PATH}]", cmd);

            CCommandOptionArray params;
            CCommandOptionArray notes;
            CCommandOptionArray errors;
            CCommandOptionArray descr;
            select_commands(cmd, params, notes, errors, descr);

            string_q route = group + toProper(cmd);
            string_q tTool = descr.size() ? descr[0].tool : "";

            goCallStream << getGoCall(tTool, route, cmd);
            goRouteStream << getGoRoute(route, cmd);

            ostringstream yamlParamStream;
            for (auto param : params) {
                string_q yp = STR_PATH_PARAM_YAML;
                replace(yp, "[{NAME}]", param.command);
                replace(yp, "[{DESCR}]", param.swagger_descr);
                replace(yp, "[{REQ}]", param.is_required ? "true" : "false");
                replace(yp, "[{TYPE}]", getTypeStr(param, "          "));
                yamlParamStream << yp << endl;
                counter.cmdCount++;
            }

            if (!yamlParamStream.str().empty()) {
                replace(yamlEntry, "[{PARAMS}]", yamlParamStream.str());
            } else {
                replace(yamlEntry, "      parameters:\n[{PARAMS}]", "");
            }
            if (descr.size()) {
                replace(yamlEntry, "[{SUMMARY}]", descr[0].swagger_descr);
                replace(yamlEntry, "[{DESCR}]", descr[0].swagger_descr);
            } else {
                replace(yamlEntry, "      summary: [{SUMMARY}]\n", "");
                replace(yamlEntry, "      description: [{DESCR}]\n", "");
            }
            replace(yamlEntry, "[{ID}]", toLower(group + "-" + cmd));

            yamlPathStream << yamlEntry;

            counter.routeCount++;
        }
    }

    writeCode("../src/go-apps/flame/cmd/routes.go");
    writeCode("../src/apps/chifra/options.cpp");
    writeCode("../src/libs/utillib/options_base.cpp");

    string_q explFolder = "../../trueblocks-explorer/";
    if (!folderExists(explFolder)) {
        LOG_WARN("Explorer folder not found: ", explFolder);
        return;
    }

    string_q yamlTemplate = configPath("makeClass/blank_openapi.yaml");
    string_q newYamlCode = asciiFileToString(yamlTemplate);
    if (!newYamlCode.empty()) {
        // LOG_INFO("Generating new yaml code");
        replace(newYamlCode, "[{PATHS}]", yamlPathStream.str());
        replace(newYamlCode, "[{TAGS}]", yamlTagStream.str());
        string_q origYaml = configPath("makeClass/openapi.yaml");
        string_q origYamlCode = asciiFileToString(origYaml);
        if (origYamlCode != newYamlCode) {
            counter.nChanged++;
            stringToAsciiFile(origYaml, newYamlCode);
            if (fileExists("../docs/openapi.yaml")) {
                stringToAsciiFile("../docs/openapi.yaml", newYamlCode);
            }
        }
    }

    string_q header = STR_HTML_CODE_HEADER;
    string_q body = STR_HTML_CODE;
    string_q footer = STR_HTML_CODE_FOOTER;
    string_q html = header + body + footer;
    for (size_t i = 12; i > 4; i--)
        replaceAll(html, string_q(i, '-'), string_q((i * 2), ' '));
    stringToAsciiFile("../docs/api.html", html);
    stringToAsciiFile(explFolder + "public/help/api.html", html);

    if (test) {
        counter.routeCount = 0;
        LOG_WARN("Testing only - openapi file not written");
    }
    LOG_INFO(cYellow, "makeClass --openapi", cOff, " processed ", counter.routeCount, "/", counter.cmdCount,
             " routes/cmds ", " (changed ", counter.nChanged, ").", string_q(40, ' '));
}

//---------------------------------------------------------------------------------------------------
string_q getGoCall(const string_q& tool, const string_q& route, const string_q& cmd) {
    ostringstream out;
    out << endl;
    out << "// " << route << " help text todo" << endl;
    out << "func " << route << "(w http.ResponseWriter, r *http.Request) {" << endl;
    if (!tool.empty()) {
        out << "\tCallOne(w, r, \"" << tool << "\", \"" << cmd << "\")" << endl;
    } else if (route == "AccountsTags" || route == "AccountsEntities") {
        out << "\tCallOne(w, r, \"ethNames\", \"" << cmd << "\")" << endl;
    } else {
        out << "\tCallOneExtra(w, r, \"chifra\", \"" << cmd << "\", \"" << cmd << "\")" << endl;
    }
    out << "}" << endl;
    return out.str();
}

//---------------------------------------------------------------------------------------------------
string_q getGoRoute(const string_q& route, const string_q& cmd) {
    ostringstream out;
    out << endl;
    out << "\tRoute{" << endl;
    out << "\t\t\"" << route << "\"," << endl;
    out << "\t\t\"GET\"," << endl;
    out << "\t\t\"/" << cmd << "\"," << endl;
    out << "\t\t" << route << "," << endl;
    out << "\t}," << endl;
    return out.str();
}

//---------------------------------------------------------------------------------------------------
void COptions::old_writeJSApiFile(void) {
    if (api) {
        LOG_INFO(cYellow, "handling api file...", cOff);
        counter = CCounter();  // reset

        CCommands commands;
        options_2_Commands(commands);

        bool firstRoute = true;
        routeStream << "{" << endl;
        for (auto route : commands.routes) {
            counter.routeCount++;
            if (!firstRoute)
                routeStream << "," << endl;
            routeStream << "  \"" << route.route << "\": {" << endl;
            bool firstCmd = true;
            for (auto command : route.commands) {
                if (command.option_type != "note") {
                    counter.cmdCount++;
                    if (command.hotkey.empty())
                        command.hotkey = "<not-set>";
                    if (command.command.empty())
                        command.command = "<not-set>";
                    if (command.data_type.empty())
                        command.data_type = "<not-set>";

                    ostringstream commandStream;
                    if (!firstCmd)
                        commandStream << "," << endl;
                    commandStream << "    ";
                    commandStream << "\"" << command.command << "\": " << command;

                    string_q str = commandStream.str();
                    replaceAll(str, "\"true\"", "true");
                    replaceAll(str, "\"false\"", "false");
                    replaceAll(str, "NOPOS", "");
                    replaceAll(str, "\"def_val\": false,", "\"def_val\": \"\",");
                    replaceAll(str, "\"def_val\": true,", "\"def_val\": \"true\",");
                    replaceAll(str, "\"def_val\": \"\"\"\",", "\"def_val\": \"\",");
                    replaceAll(str, "<not-set>", "");
                    replaceReverse(str, "\n", "");
                    routeStream << str;
                    firstCmd = false;
                }
            }
            routeStream << endl << "  }";
            firstRoute = false;
        }
        routeStream << endl << "}" << endl;
        if (!test && folderExists("../../trueblocks-explorer/api/"))
            stringToAsciiFile("../../trueblocks-explorer/api/api_options.json", routeStream.str());

        if (test) {
            counter.routeCount = 0;
            LOG_WARN("Testing only - api file not written");
        }
        LOG_INFO(cYellow, "makeClass --api", cOff, " processed ", counter.routeCount, " routes and ", counter.cmdCount,
                 " commands.", string_q(40, ' '));
    }

    writeOpenApiFile();
}

//---------------------------------------------------------------------------------------------------
void COptions::options_2_Commands(CCommands& commands) {
    CApiRoute curRoute;
    for (auto option : optionArray) {
        if (curRoute.route != option.api_route) {
            if (!curRoute.route.empty())
                commands.routes.push_back(curRoute);
            curRoute = CApiRoute();
            curRoute.route = option.api_route;
        }
        curRoute.commands.push_back(option);
    }
    commands.routes.push_back(curRoute);
    expContext().lev = 2;
    HIDE_FIELD(CCommandOption, "is_customizable");
}

//---------------------------------------------------------------------------------------------------
void COptions::select_commands(const string_q& cmd, CCommandOptionArray& cmds, CCommandOptionArray& notes,
                               CCommandOptionArray& errors, CCommandOptionArray& descr) {
    for (auto option : optionArray) {
        if (option.api_route == cmd) {
            if (option.option_type == "description") {
                descr.push_back(option);
            } else if (option.option_type == "note") {
                notes.push_back(option);
            } else if (option.option_type == "error") {
                errors.push_back(option);
            } else if (option.option_type != "deprecated") {
                cmds.push_back(option);
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------
string_q getTypeStr(const CCommandOption& opt, const string& lead) {
    string_q str_head = lead + "type: string";
    string_q num_head = lead + "type: number";
    string_q bool_head = lead + "type: boolean";
    string_q enum_head = lead + "type: string\n" + lead + "enum:\n";

    if (contains(opt.data_type, "list")) {
        if (contains(opt.data_type, "enum")) {
            string_q e = substitute(substitute(opt.data_type, "list<", ""), ">", "");
            string_q str = substitute(substitute(substitute(e, "*", ""), "enum[", ""), "]", "");
            CStringArray opts;
            explode(opts, str, '|');
            ostringstream os;
            for (auto o : opts) {
                if (isNumeral(o)) {
                    os << substitute("            - \"[{VAL}]\"\n", "[{VAL}]", o);
                } else {
                    os << substitute("            - [{VAL}]\n", "[{VAL}]", o);
                }
            }
            string_q str_array_enum =
                "          type: array\n"
                "          items:\n"
                "            type: string\n"
                "            enum:\n";
            return str_array_enum + trim(os.str(), '\n');
        } else {
            string_q str_array =
                "          type: array\n"
                "          items:\n"
                "            type: string";
            return str_array;
        }
    }

    if (contains(opt.data_type, "boolean")) {
        return bool_head;

    } else if (contains(opt.data_type, "uint") || contains(opt.data_type, "double")) {
        return num_head;

    } else if (contains(opt.data_type, "enum")) {
        string_q str = substitute(substitute(substitute(opt.data_type, "*", ""), "enum[", ""), "]", "");
        CStringArray opts;
        explode(opts, str, '|');
        ostringstream os;
        for (auto o : opts) {
            if (isNumeral(o)) {
                os << substitute(lead + "- \"[{VAL}]\"\n", "[{VAL}]", o);
            } else {
                os << substitute(lead + "- [{VAL}]\n", "[{VAL}]", o);
            }
        }
        return enum_head + trim(os.str(), '\n');
    }
    return str_head;
}

static CStringArray header;
//---------------------------------------------------------------------------------------------------
bool getGroups(const char* str, void* data) {
    string_q line = str;
    replaceAny(line, "\n\r", "");
    if (header.empty()) {
        explode(header, line, ',');
        return true;
    }

    CCommandOptionArray* array = (CCommandOptionArray*)data;
    CCommandOption option;
    option.parseCSV(header, line);
    array->push_back(option);

    return true;
}

//---------------------------------------------------------------------------------------------------
bool loadEndpoints(CEndpointMap& endpointMap, CCommandOptionArray& endpointArray) {
    forEveryLineInAsciiFile("../src/cmd-line-endpoints.csv", getGroups, &endpointArray);

    for (auto g : endpointArray) {
        endpointMap[g.num].group = substitute(g.group, " ", "");
        if (g.is_visible) {
            if (!endpointMap[g.num].route_list.empty())
                endpointMap[g.num].route_list += ",";
            endpointMap[g.num].route_list += trim(g.api_route);
        }
        if (endpointMap[g.num].description.empty())
            endpointMap[g.num].description = trim(g.description);
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
const char* STR_TAG_ENTRY_YAML =
    "- name: [{NAME}]\n"
    "  description: [{DESCR}]\n";

//---------------------------------------------------------------------------------------------------
const char* STR_PATH_ENTRY_YAML =
    "  /[{PATH}]:\n"
    "    get:\n"
    "      tags:\n"
    "      - [{TAGS}]\n"
    "      summary: [{SUMMARY}]\n"
    "      description: [{DESCR}]\n"
    "      operationId: [{ID}]\n"
    "      parameters:\n"
    "[{PARAMS}]"
    "      responses:\n"
    "        \"200\":\n"
    "          description: status of the scraper\n"
    "          content:\n"
    "            application/json:\n"
    "              schema:\n"
    "                type: array\n"
    "                items:\n"
    "                  $ref: '#/components/schemas/response'\n"
    "        \"400\":\n"
    "          description: bad input parameter\n";

//---------------------------------------------------------------------------------------------------
const char* STR_PATH_PARAM_YAML =
    "      - name: [{NAME}]\n"
    "        in: query\n"
    "        description: [{DESCR}]\n"
    "        required: [{REQ}]\n"
    "        style: form\n"
    "        explode: true\n"
    "        schema:\n"
    "[{TYPE}]";

//---------------------------------------------------------------------------------------------------
// clang-format off
const char* STR_HTML_CODE_HEADER =
    "<!DOCTYPE html>\n"
    "<html lang=\"en\">\n"
    "  <head>\n"
    "    <meta charset=\"UTF-8\" />\n"
    "    <title>Swagger UI</title>\n"
    "    <link href=\"https://fonts.googleapis.com/css?family=Open+Sans:400,700|Source+Code+Pro:300,600|Titillium+Web:400,600,700\" rel=\"stylesheet\" />\n"
    "    <link rel=\"stylesheet\" type=\"text/css\" href=\"https://cdnjs.cloudflare.com/ajax/libs/swagger-ui/3.24.2/swagger-ui.css\" />\n"
    "    <style>\n"
    "      html {\n"
    "        box-sizing: border-box;\n"
    "        overflow: -moz-scrollbars-vertical;\n"
    "        overflow-y: scroll;\n"
    "      }\n"
    "      *,\n"
    "      *:before,\n"
    "      *:after {\n"
    "        box-sizing: inherit;\n"
    "      }\n"
    "      body {\n"
    "        margin: 0;\n"
    "        background: #fafafa;\n"
    "      }\n"
    "    </style>\n"
    "  </head>\n"
    "  <body>\n"
    "    <div id=\"swagger-ui\"></div>\n"
    "    <script src=\"https://cdnjs.cloudflare.com/ajax/libs/swagger-ui/3.24.2/swagger-ui-bundle.js\"></script>\n"
    "    <script src=\"https://cdnjs.cloudflare.com/ajax/libs/swagger-ui/3.24.2/swagger-ui-standalone-preset.js\"></script>\n"
    "    <script>\n"
    "      window.onload = function () {\n"
    "        var spec = {\n"
    "          openapi: '3.0.0',\n"
    "          info: {\n"
    "            title: 'TrueBlocks API',\n"
    "            description: 'An API for accessing cached Ethereum blockchain data and accounts',\n"
    "            contact: {email: 'jrush@quickblocks.io'},\n"
    "            license: {name: 'Apache 2.0', url: 'http://www.apache.org/licenses/LICENSE-2.0.html'},\n"
    "            version: '0.8.04',\n"
    "          },\n"
    "          servers: [\n"
    "            {url: 'http://localhost:8080', description: 'Local endpoints'},\n"
    "            {\n"
    "              url: 'https://virtserver.swaggerhub.com/trueblocks/TrueBlocks/0.8.04',\n"
    "              description: 'SwaggerHub API Auto Mocking',\n"
    "            },\n"
    "          ],\n"
    "          tags: [\n"
    "            {name: 'Accounts', description: 'Access and cache transactional data'},\n"
    "            {name: 'Chain Data', description: 'Access and cache blockchain-related data'},\n"
    "            {name: 'Chain State', description: 'Access to account and token state'},\n"
    "            {name: 'Admin', description: 'Control the scraper and build the index'},\n"
    "            {name: 'Other', description: 'Access to other and external data'},\n"
    "          ],\n"
    "-----paths: {\n";

const char* STR_HTML_CODE =
    "------'/list': {\n"
    "-------get: {\n"
    "--------tags: ['Accounts'],\n"
    "--------summary: 'Add or remove monitors for a given Ethereum address (or collection of addresses).',\n"
    "--------description: 'Add or remove monitors for a given Ethereum address (or collection of addresses).',\n"
    "--------operationId: 'accounts-list',\n"
    "--------parameters: [\n"
    "---------{\n"
    "----------name: 'addrs',\n"
    "----------in: 'query',\n"
    "----------description: 'one or more Ethereum addresses',\n"
    "----------required: true,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'array', items: {type: 'string'}},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'staging',\n"
    "----------in: 'query',\n"
    "----------description: 'enable search of staging (not yet finalized) folder',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'unripe',\n"
    "----------in: 'query',\n"
    "----------description: 'enable search of unripe (neither staged nor finalized) folder (requires --staging)',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'blooms',\n"
    "----------in: 'query',\n"
    "----------description: 'process query by first using bloom filter and, if hit, downloading index chunk from remote',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'start',\n"
    "----------in: 'query',\n"
    "----------description: 'this value is ignored but remains for backward compatibility',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'string'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'end',\n"
    "----------in: 'query',\n"
    "----------description: 'this value is ignored but remains for backward compatibility',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'string'},\n"
    "---------},\n"
    "--------],\n"
    "--------responses: {\n"
    "---------'200': {\n"
    "----------description: 'status of the scraper',\n"
    "----------content: { 'application/json': {schema: {type: 'array', items: {$ref: '#/components/schemas/response'}}} }, },\n"
    "---------'400': {description: 'bad input parameter'},\n"
    "--------},\n"
    "-------},\n"
    "------},\n"
    "------'/export': {\n"
    "-------get: {\n"
    "--------tags: ['Accounts'],\n"
    "--------summary: 'Export full detail of transactions for one or more Ethereum addresses.',\n"
    "--------description: 'Export full detail of transactions for one or more Ethereum addresses.',\n"
    "--------operationId: 'accounts-export',\n"
    "--------parameters: [\n"
    "---------{\n"
    "----------name: 'addrs',\n"
    "----------in: 'query',\n"
    "----------description: 'one or more addresses (0x...) to export',\n"
    "----------required: true,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'array', items: {type: 'string'}},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'topics',\n"
    "----------in: 'query',\n"
    "----------description: 'filter by one or more logs topics (only for --logs option)',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'array', items: {type: 'string'}},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'appearances',\n"
    "----------in: 'query',\n"
    "----------description: 'export a list of appearances',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'receipts',\n"
    "----------in: 'query',\n"
    "----------description: 'export receipts instead of transaction list',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'logs',\n"
    "----------in: 'query',\n"
    "----------description: 'export logs instead of transaction list',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'traces',\n"
    "----------in: 'query',\n"
    "----------description: 'export traces instead of transaction list',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'statements',\n"
    "----------in: 'query',\n"
    "----------description: 'export reconcilations instead of transaction list',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'accounting',\n"
    "----------in: 'query',\n"
    "----------description: 'export accounting records instead of transaction list',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'articulate',\n"
    "----------in: 'query',\n"
    "----------description: 'articulate transactions, traces, logs, and outputs',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'write_txs',\n"
    "----------in: 'query',\n"
    "----------description: 'write transactions to the cache (see notes)',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'write_traces',\n"
    "----------in: 'query',\n"
    "----------description: 'write traces to the cache (see notes)',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'skip_ddos',\n"
    "----------in: 'query',\n"
    "----------description: \"toggle skipping over 2016 dDos transactions ('on' by default)\",\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'max_traces',\n"
    "----------in: 'query',\n"
    "----------description: 'if --skip_ddos is on, this many traces defines what a ddos transaction is (default = 250)',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'number'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'freshen',\n"
    "----------in: 'query',\n"
    "----------description: 'freshen but do not print the exported data',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'freshen_max',\n"
    "----------in: 'query',\n"
    "----------description: 'maximum number of records to process for --freshen option',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'number'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'factory',\n"
    "----------in: 'query',\n"
    "----------description: 'scan for contract creations from the given address(es) and report address of those contracts',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'emitter',\n"
    "----------in: 'query',\n"
    "----------description: 'available for --logs option only, export will only export if the address emitted the event',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'count',\n"
    "----------in: 'query',\n"
    "----------description: 'only available for --appearances mode, if present return only the number of records',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'first_record',\n"
    "----------in: 'query',\n"
    "----------description: 'the first record to process',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'string'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'max_records',\n"
    "----------in: 'query',\n"
    "----------description: 'the maximum number of records to process before reporting',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'string'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'staging',\n"
    "----------in: 'query',\n"
    "----------description: 'ignored (preserved for backwards compatibility)',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'unripe',\n"
    "----------in: 'query',\n"
    "----------description: 'ignored (preserved for backwards compatibility)',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "--------],\n"
    "--------responses: {\n"
    "---------'200': {\n"
    "----------description: 'status of the scraper',\n"
    "----------content: { 'application/json': {schema: {type: 'array', items: {$ref: '#/components/schemas/response'}}} },\n"
    "---------},\n"
    "---------'400': {description: 'bad input parameter'},\n"
    "--------},\n"
    "-------},\n"
    "------},\n"
    "------'/monitors': {\n"
    "-------get: {\n"
    "--------tags: ['Accounts'],\n"
    "--------operationId: 'accounts-rm',\n"
    "--------parameters: [\n"
    "---------{\n"
    "----------name: 'rm',\n"
    "----------in: 'query',\n"
    "----------description: 'process the request to delete, undelete, or remove monitors',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "--------],\n"
    "--------responses: {\n"
    "---------'200': {\n"
    "----------description: 'status of the scraper',\n"
    "----------content: { 'application/json': {schema: {type: 'array', items: {$ref: '#/components/schemas/response'}}} },\n"
    "---------},\n"
    "---------'400': {description: 'bad input parameter'},\n"
    "--------},\n"
    "-------},\n"
    "------},\n"
    "------'/names': {\n"
    "-------get: {\n"
    "--------tags: ['Accounts'],\n"
    "--------summary: 'Query addresses and/or names of well known accounts.',\n"
    "--------description: 'Query addresses and/or names of well known accounts.',\n"
    "--------operationId: 'accounts-names',\n"
    "--------parameters: [\n"
    "---------{\n"
    "----------name: 'terms',\n"
    "----------in: 'query',\n"
    "----------description: 'a space separated list of one or more search terms',\n"
    "----------required: true,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'array', items: {type: 'string'}},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'expand',\n"
    "----------in: 'query',\n"
    "----------description: 'expand search to include all fields (default searches name, address, and symbol only)',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'match_case',\n"
    "----------in: 'query',\n"
    "----------description: 'do case-sensitive search',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'all',\n"
    "----------in: 'query',\n"
    "----------description: 'include all accounts in the search',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'custom',\n"
    "----------in: 'query',\n"
    "----------description: 'include your custom named accounts',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'prefund',\n"
    "----------in: 'query',\n"
    "----------description: 'include prefund accounts',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'named',\n"
    "----------in: 'query',\n"
    "----------description: 'include well know token and airdrop addresses in the search',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'other',\n"
    "----------in: 'query',\n"
    "----------description: 'export other addresses if found',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'addr',\n"
    "----------in: 'query',\n"
    "----------description: 'display only addresses in the results (useful for scripting)',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'to_custom',\n"
    "----------in: 'query',\n"
    "----------description: 'for editCmd only, is the edited name a custom name or not',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'clean',\n"
    "----------in: 'query',\n"
    "----------description: 'clean the data (addrs to lower case, sort by addr)',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "--------],\n"
    "--------responses: {\n"
    "---------'200': {\n"
    "----------description: 'status of the scraper',\n"
    "----------content: { 'application/json': {schema: {type: 'array', items: {$ref: '#/components/schemas/response'}}} },\n"
    "---------},\n"
    "---------'400': {description: 'bad input parameter'},\n"
    "--------},\n"
    "-------},\n"
    "------},\n"
    "------'/abis': {\n"
    "-------get: {\n"
    "--------tags: ['Accounts'],\n"
    "--------summary: 'Fetches the ABI for a smart contract.',\n"
    "--------description: 'Fetches the ABI for a smart contract.',\n"
    "--------operationId: 'accounts-abis',\n"
    "--------parameters: [\n"
    "---------{\n"
    "----------name: 'addrs',\n"
    "----------in: 'query',\n"
    "----------description: 'list of one or more smart contracts whose ABI to grab from EtherScan',\n"
    "----------required: true,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'array', items: {type: 'string'}},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'known',\n"
    "----------in: 'query',\n"
    "----------description: \"load common 'known' ABIs from cache\",\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'sol',\n"
    "----------in: 'query',\n"
    "----------description: 'file name of .sol file from which to create a new known abi (without .sol)',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'string'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'find',\n"
    "----------in: 'query',\n"
    "----------description: 'try to search for a function declaration given a four byte code',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'string'},\n"
    "---------},\n"
    "--------],\n"
    "--------responses: {\n"
    "---------'200': {\n"
    "----------description: 'status of the scraper',\n"
    "----------content: { 'application/json': {schema: {type: 'array', items: {$ref: '#/components/schemas/response'}}} },\n"
    "---------},\n"
    "---------'400': {description: 'bad input parameter'},\n"
    "--------},\n"
    "-------},\n"
    "------},\n"
    "------'/entities': {\n"
    "-------get: {\n"
    "--------tags: ['Accounts'],\n"
    "--------operationId: 'accounts-collections',\n"
    "--------parameters: [\n"
    "---------{\n"
    "----------name: 'collections',\n"
    "----------in: 'query',\n"
    "----------description: 'display collections data',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "--------],\n"
    "--------responses: {\n"
    "---------'200': {\n"
    "----------description: 'status of the scraper',\n"
    "----------content: { 'application/json': {schema: {type: 'array', items: {$ref: '#/components/schemas/response'}}} },\n"
    "---------},\n"
    "---------'400': {description: 'bad input parameter'},\n"
    "--------},\n"
    "-------},\n"
    "------},\n"
    "------'/tags': {\n"
    "-------get: {\n"
    "--------tags: ['Accounts'],\n"
    "--------operationId: 'accounts-tags',\n"
    "--------parameters: [\n"
    "---------{\n"
    "----------name: 'tags',\n"
    "----------in: 'query',\n"
    "----------description: 'export the list of tags and subtags only',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "--------],\n"
    "--------responses: {\n"
    "---------'200': {\n"
    "----------description: 'status of the scraper',\n"
    "----------content: { 'application/json': {schema: {type: 'array', items: {$ref: '#/components/schemas/response'}}} },\n"
    "---------},\n"
    "---------'400': {description: 'bad input parameter'},\n"
    "--------},\n"
    "-------},\n"
    "------},\n"
    "------'/blocks': {\n"
    "-------get: {\n"
    "--------tags: ['Chain Data'],\n"
    "--------summary: 'Returns block(s) from local cache or directly from a running node.',\n"
    "--------description: 'Returns block(s) from local cache or directly from a running node.',\n"
    "--------operationId: 'data-blocks',\n"
    "--------parameters: [\n"
    "---------{\n"
    "----------name: 'blocks',\n"
    "----------in: 'query',\n"
    "----------description: 'a space-separated list of one or more blocks to retrieve',\n"
    "----------required: true,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'array', items: {type: 'string'}},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'hashes',\n"
    "----------in: 'query',\n"
    "----------description: 'display only transaction hashes, default is to display full transaction detail',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'apps',\n"
    "----------in: 'query',\n"
    "----------description: 'display all address appearances included in the block',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'uniq',\n"
    "----------in: 'query',\n"
    "----------description: 'display only uniq addresses found per block',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'uniq_tx',\n"
    "----------in: 'query',\n"
    "----------description: 'display only uniq addresses found per transaction',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'count',\n"
    "----------in: 'query',\n"
    "----------description: 'display counts of appearances (for --apps, --uniq, or --uniq_tx) or transactions',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'uncles',\n"
    "----------in: 'query',\n"
    "----------description: 'display uncle blocks (if any) instead of the requested block',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'force',\n"
    "----------in: 'query',\n"
    "----------description: 'force a re-write of the block to the cache',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'trace',\n"
    "----------in: 'query',\n"
    "----------description: 'export the traces from the block as opposed to the block data',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "--------],\n"
    "--------responses: {\n"
    "---------'200': {\n"
    "----------description: 'status of the scraper',\n"
    "----------content: { 'application/json': {schema: {type: 'array', items: {$ref: '#/components/schemas/response'}}} },\n"
    "---------},\n"
    "---------'400': {description: 'bad input parameter'},\n"
    "--------},\n"
    "-------},\n"
    "------},\n"
    "------'/transactions': {\n"
    "-------get: {\n"
    "--------tags: ['Chain Data'],\n"
    "--------summary: 'Retrieve a transaction from the cache or the node.',\n"
    "--------description: 'Retrieve a transaction from the cache or the node.',\n"
    "--------operationId: 'data-transactions',\n"
    "--------parameters: [\n"
    "---------{\n"
    "----------name: 'transactions',\n"
    "----------in: 'query',\n"
    "----------description: 'a space-separated list of one or more transaction identifiers (tx_hash, bn.txID, blk_hash.txID)',\n"
    "----------required: true,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'array', items: {type: 'string'}},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'articulate',\n"
    "----------in: 'query',\n"
    "----------description: \"articulate the transactions if an ABI is found for the 'to' address\",\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'trace',\n"
    "----------in: 'query',\n"
    "----------description: \"display the transaction's trace\",\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'force',\n"
    "----------in: 'query',\n"
    "----------description: 'force the results into the tx cache',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'uniq',\n"
    "----------in: 'query',\n"
    "----------description: 'display a list of uniq addresses found in this transaction',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "--------],\n"
    "--------responses: {\n"
    "---------'200': {\n"
    "----------description: 'status of the scraper',\n"
    "----------content: { 'application/json': {schema: {type: 'array', items: {$ref: '#/components/schemas/response'}}} },\n"
    "---------},\n"
    "---------'400': {description: 'bad input parameter'},\n"
    "--------},\n"
    "-------},\n"
    "------},\n"
    "------'/receipts': {\n"
    "-------get: {\n"
    "--------tags: ['Chain Data'],\n"
    "--------summary: \"Retrieve a transaction's receipt from the cache or the node.\",\n"
    "--------description: \"Retrieve a transaction's receipt from the cache or the node.\",\n"
    "--------operationId: 'data-receipts',\n"
    "--------parameters: [\n"
    "---------{\n"
    "----------name: 'transactions',\n"
    "----------in: 'query',\n"
    "----------description: 'a space-separated list of one or more transaction identifiers (tx_hash, bn.txID, blk_hash.txID)',\n"
    "----------required: true,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'array', items: {type: 'string'}},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'articulate',\n"
    "----------in: 'query',\n"
    "----------description: \"articulate the transactions if an ABI is found for the 'to' address\",\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'logs',\n"
    "----------in: 'query',\n"
    "----------description: \"display the receipt's logs\",\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "--------],\n"
    "--------responses: {\n"
    "---------'200': {\n"
    "----------description: 'status of the scraper',\n"
    "----------content: { 'application/json': {schema: {type: 'array', items: {$ref: '#/components/schemas/response'}}} },\n"
    "---------},\n"
    "---------'400': {description: 'bad input parameter'},\n"
    "--------},\n"
    "-------},\n"
    "------},\n"
    "------'/logs': {\n"
    "-------get: {\n"
    "--------tags: ['Chain Data'],\n"
    "--------summary: \"Retrieve a transaction's logs from the cache or the node.\",\n"
    "--------description: \"Retrieve a transaction's logs from the cache or the node.\",\n"
    "--------operationId: 'data-logs',\n"
    "--------parameters: [\n"
    "---------{\n"
    "----------name: 'transactions',\n"
    "----------in: 'query',\n"
    "----------description: 'a space-separated list of one or more transaction identifiers (tx_hash, bn.txID, blk_hash.txID)',\n"
    "----------required: true,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'array', items: {type: 'string'}},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'articulate',\n"
    "----------in: 'query',\n"
    "----------description: \"articulate the transactions if an ABI is found for the 'to' address\",\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "--------],\n"
    "--------responses: {\n"
    "---------'200': {\n"
    "----------description: 'status of the scraper',\n"
    "----------content: { 'application/json': {schema: {type: 'array', items: {$ref: '#/components/schemas/response'}}} },\n"
    "---------},\n"
    "---------'400': {description: 'bad input parameter'},\n"
    "--------},\n"
    "-------},\n"
    "------},\n"
    "------'/traces': {\n"
    "-------get: {\n"
    "--------tags: ['Chain Data'],\n"
    "--------summary: \"Retrieve a transaction's traces from the cache or the node.\",\n"
    "--------description: \"Retrieve a transaction's traces from the cache or the node.\",\n"
    "--------operationId: 'data-traces',\n"
    "--------parameters: [\n"
    "---------{\n"
    "----------name: 'transactions',\n"
    "----------in: 'query',\n"
    "----------description: 'a space-separated list of one or more transaction identifiers (tx_hash, bn.txID, blk_hash.txID)',\n"
    "----------required: true,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'array', items: {type: 'string'}},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'articulate',\n"
    "----------in: 'query',\n"
    "----------description: \"articulate the transactions if an ABI is found for the 'to' address\",\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'count',\n"
    "----------in: 'query',\n"
    "----------description: 'show the number of traces for the transaction only (fast)',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'skip_ddos',\n"
    "----------in: 'query',\n"
    "----------description: \"skip over 2018 ddos during export ('on' by default)\",\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'max_traces',\n"
    "----------in: 'query',\n"
    "----------description: 'if --skip_ddos is on, this many traces defines what a ddos transaction is (default = 250)',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'number'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'filter',\n"
    "----------in: 'query',\n"
    "----------description: 'Call trace_filter with bang-seperated string fromBlk!toBlk[!fromAddr[!toAddr[!after[!count]]]]',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'string'},\n"
    "---------},\n"
    "--------],\n"
    "--------responses: {\n"
    "---------'200': {\n"
    "----------description: 'status of the scraper',\n"
    "----------content: { 'application/json': {schema: {type: 'array', items: {$ref: '#/components/schemas/response'}}} },\n"
    "---------},\n"
    "---------'400': {description: 'bad input parameter'},\n"
    "--------},\n"
    "-------},\n"
    "------},\n"
    "------'/when': {\n"
    "-------get: {\n"
    "--------tags: ['Chain Data'],\n"
    "--------summary: \"Finds block based on date, blockNum, timestamp, or 'special'.\",\n"
    "--------description: \"Finds block based on date, blockNum, timestamp, or 'special'.\",\n"
    "--------operationId: 'data-when',\n"
    "--------parameters: [\n"
    "---------{\n"
    "----------name: 'block_list',\n"
    "----------in: 'query',\n"
    "----------description: 'one or more dates, block numbers, hashes, or special named blocks (see notes)',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'array', items: {type: 'string'}},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'list',\n"
    "----------in: 'query',\n"
    "----------description: \"export a list of the 'special' blocks\",\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'timestamps',\n"
    "----------in: 'query',\n"
    "----------description: 'ignore other options and generate timestamps only',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'skip',\n"
    "----------in: 'query',\n"
    "----------description: 'only applicable if --timestamps is on, the step between block numbers in the export',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'number'},\n"
    "---------},\n"
    "--------],\n"
    "--------responses: {\n"
    "---------'200': {\n"
    "----------description: 'status of the scraper',\n"
    "----------content: { 'application/json': {schema: {type: 'array', items: {$ref: '#/components/schemas/response'}}} },\n"
    "---------},\n"
    "---------'400': {description: 'bad input parameter'},\n"
    "--------},\n"
    "-------},\n"
    "------},\n"
    "------'/state': {\n"
    "-------get: {\n"
    "--------tags: ['Chain State'],\n"
    "--------summary: 'Retrieve the balance of one or more address at the given block(s).',\n"
    "--------description: 'Retrieve the balance of one or more address at the given block(s).',\n"
    "--------operationId: 'state-state',\n"
    "--------parameters: [\n"
    "---------{\n"
    "----------name: 'addrs',\n"
    "----------in: 'query',\n"
    "----------description: 'one or more addresses (0x...) from which to retrieve balances',\n"
    "----------required: true,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'array', items: {type: 'string'}},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'blocks',\n"
    "----------in: 'query',\n"
    "----------description: \"an optional list of one or more blocks at which to report balances, defaults to 'latest'\",\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'array', items: {type: 'string'}},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'parts',\n"
    "----------in: 'query',\n"
    "----------description: 'control which state to export',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: { type: 'array',items: {type: 'string', enum: ['none', 'some', 'all', 'balance', 'nonce', 'code', 'storage', 'deployed', 'accttype'] }\n"
    "----------},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'changes',\n"
    "----------in: 'query',\n"
    "----------description: 'only report a balance when it changes from one block to the next',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'no_zero',\n"
    "----------in: 'query',\n"
    "----------description: 'suppress the display of zero balance accounts',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'call',\n"
    "----------in: 'query',\n"
    "----------description: 'a bang-separated string consisting of address!4-byte!bytes',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'string'},\n"
    "---------},\n"
    "--------],\n"
    "--------responses: {\n"
    "---------'200': {\n"
    "----------description: 'status of the scraper',\n"
    "----------content: { 'application/json': {schema: {type: 'array', items: {$ref: '#/components/schemas/response'}}} },\n"
    "---------},\n"
    "---------'400': {description: 'bad input parameter'},\n"
    "--------},\n"
    "-------},\n"
    "------},\n"
    "------'/tokens': {\n"
    "-------get: {\n"
    "--------tags: ['Chain State'],\n"
    "--------summary: 'Retrieve token balances for one or more address at given block(s).',\n"
    "--------description: 'Retrieve token balances for one or more address at given block(s).',\n"
    "--------operationId: 'state-tokens',\n"
    "--------parameters: [\n"
    "---------{\n"
    "----------name: 'addrs2',\n"
    "----------in: 'query',\n"
    "----------description: 'two or more addresses (0x...), the first is an ERC20 token, balances for the rest are reported',\n"
    "----------required: true,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'array', items: {type: 'string'}},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'blocks',\n"
    "----------in: 'query',\n"
    "----------description: \"an optional list of one or more blocks at which to report balances, defaults to 'latest'\",\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'array', items: {type: 'string'}},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'parts',\n"
    "----------in: 'query',\n"
    "----------description: 'one or more parts of the token information to retreive',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: { type: 'array',items: {type: 'string', enum: ['name', 'symbol', 'decimals', 'totalSupply', 'version', 'none', 'all'] }\n"
    "----------},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'by_acct',\n"
    "----------in: 'query',\n"
    "----------description: 'consider each address an ERC20 token except the last, whose balance is reported for each token',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'no_zero',\n"
    "----------in: 'query',\n"
    "----------description: 'suppress the display of zero balance accounts',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "--------],\n"
    "--------responses: {\n"
    "---------'200': {\n"
    "----------description: 'status of the scraper',\n"
    "----------content: { 'application/json': {schema: {type: 'array', items: {$ref: '#/components/schemas/response'}}} },\n"
    "---------},\n"
    "---------'400': {description: 'bad input parameter'},\n"
    "--------},\n"
    "-------},\n"
    "------},\n"
    "------'/status': {\n"
    "-------get: {\n"
    "--------tags: ['Admin'],\n"
    "--------summary: 'Report on status of one or more TrueBlocks caches.',\n"
    "--------description: 'Report on status of one or more TrueBlocks caches.',\n"
    "--------operationId: 'admin-status',\n"
    "--------parameters: [\n"
    "---------{\n"
    "----------name: 'modes',\n"
    "----------in: 'query',\n"
    "----------description: 'the type of status info to retrieve',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: { type: 'array', items: {type: 'string', enum: ['index', 'monitors', 'collections', 'names', 'abis', 'caches', 'some', 'all'] }\n"
    "----------},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'details',\n"
    "----------in: 'query',\n"
    "----------description: 'include details about items found in monitors, slurps, abis, or price caches',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'types',\n"
    "----------in: 'query',\n"
    "----------description: 'for cache mode only, which type(s) of cache to report',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: { type: 'array', items: {type: 'string', enum: ['blocks', 'transactions', 'traces', 'slurps', 'prices', 'all']}},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'depth',\n"
    "----------in: 'query',\n"
    "----------description: 'for cache mode only, number of levels deep to report',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'number'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'report',\n"
    "----------in: 'query',\n"
    "----------description: 'show a summary of the current status of TrueBlocks (deprecated)',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'terse',\n"
    "----------in: 'query',\n"
    "----------description: 'show a terse summary report',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'get_config',\n"
    "----------in: 'query',\n"
    "----------description: 'returns JSON data of the editable configuration file items',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'set_config',\n"
    "----------in: 'query',\n"
    "----------description: 'accepts JSON in an env variable and writes it to configuration files',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'start',\n"
    "----------in: 'query',\n"
    "----------description: 'first block to process (inclusive)',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'string'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'end',\n"
    "----------in: 'query',\n"
    "----------description: 'last block to process (inclusive)',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'string'},\n"
    "---------},\n"
    "--------],\n"
    "--------responses: {\n"
    "---------'200': {\n"
    "----------description: 'status of the scraper',\n"
    "----------content: { 'application/json': {schema: {type: 'array', items: {$ref: '#/components/schemas/response'}}} },\n"
    "---------},\n"
    "---------'400': {description: 'bad input parameter'},\n"
    "--------},\n"
    "-------},\n"
    "------},\n"
    "------'/serve': {\n"
    "------},\n"
    "------'/scrape': {\n"
    "-------get: {\n"
    "--------tags: ['Admin'],\n"
    "--------summary: 'Decentralized blockchain scraper and block cache.',\n"
    "--------description: 'Decentralized blockchain scraper and block cache.',\n"
    "--------operationId: 'admin-scrape',\n"
    "--------parameters: [\n"
    "---------{\n"
    "----------name: 'mode',\n"
    "----------in: 'query',\n"
    "----------description: 'control the block and account scrapers',\n"
    "----------required: true,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: { type: 'string', enum: ['run', 'quit', 'pause', 'restart'] }\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'tool',\n"
    "----------in: 'query',\n"
    "----------description: 'process the index, monitors, or both (none means process timestamps only)',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: { type: 'array', items: {type: 'string', enum: ['monitors', 'index', 'none', 'both']}},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'n_blocks',\n"
    "----------in: 'query',\n"
    "----------description: 'maximum number of blocks to process (defaults to 5000)',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'number'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'n_block_procs',\n"
    "----------in: 'query',\n"
    "----------description: 'number of block channels for blaze',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'number'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'n_addr_procs',\n"
    "----------in: 'query',\n"
    "----------description: 'number of address channels for blaze',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'number'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'pin',\n"
    "----------in: 'query',\n"
    "----------description: 'pin new chunks (and blooms) to IPFS (requires Pinata key and running IPFS node)',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'publish',\n"
    "----------in: 'query',\n"
    "----------description: 'publish the hash of the pin manifest to the UnchainedIndex smart contract',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'sleep',\n"
    "----------in: 'query',\n"
    "----------description: 'the number of seconds to sleep between passes (default 14)',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'number'},\n"
    "---------},\n"
    "--------],\n"
    "--------responses: {\n"
    "---------'200': {\n"
    "----------description: 'status of the scraper',\n"
    "----------content: { 'application/json': {schema: {type: 'array', items: {$ref: '#/components/schemas/response'}}} },\n"
    "---------},\n"
    "---------'400': {description: 'bad input parameter'},\n"
    "--------},\n"
    "-------},\n"
    "------},\n"
    "------'/init': {\n"
    "------},\n"
    "------'/pins': {\n"
    "------},\n"
    "------'/quotes': {\n"
    "-------get: {\n"
    "--------tags: ['Other'],\n"
    "--------summary: 'Freshen and/or display Ethereum price data.',\n"
    "--------description: 'Freshen and/or display Ethereum price data.',\n"
    "--------operationId: 'other-quotes',\n"
    "--------parameters: [\n"
    "---------{\n"
    "----------name: 'freshen',\n"
    "----------in: 'query',\n"
    "----------description: 'Freshen price database (append new data)',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'period',\n"
    "----------in: 'query',\n"
    "----------description: 'increment of display',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: { type: 'string', enum: ['5', '15', '30', '60', '120', '240', '1440', '10080', 'hourly', 'daily', 'weekly'],},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'pair',\n"
    "----------in: 'query',\n"
    "----------description: 'which price pair to freshen or list (see Poloniex)',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'string'},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'feed',\n"
    "----------in: 'query',\n"
    "----------description: 'the feed for the price data',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: { type: 'string', enum: ['poloniex', 'maker', 'tellor'] }\n"
    "---------},\n"
    "--------],\n"
    "--------responses: {\n"
    "---------'200': {\n"
    "----------description: 'status of the scraper',\n"
    "----------content: { 'application/json': {schema: {type: 'array', items: {$ref: '#/components/schemas/response'}}} },\n"
    "---------},\n"
    "---------'400': {description: 'bad input parameter'},\n"
    "--------},\n"
    "-------},\n"
    "------},\n"
    "------'/explore': {\n"
    "------},\n"
    "------'/slurp': {\n"
    "-------get: {\n"
    "--------tags: ['Other'],\n"
    "--------summary: 'Fetches data from EtherScan for an arbitrary address.',\n"
    "--------description: 'Fetches data from EtherScan for an arbitrary address.',\n"
    "--------operationId: 'other-slurp',\n"
    "--------parameters: [\n"
    "---------{\n"
    "----------name: 'addrs',\n"
    "----------in: 'query',\n"
    "----------description: 'one or more addresses to slurp from Etherscan',\n"
    "----------required: true,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'array', items: {type: 'string'}},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'blocks',\n"
    "----------in: 'query',\n"
    "----------description: 'an optional range of blocks to slurp',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'array', items: {type: 'string'}},\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'types',\n"
    "----------in: 'query',\n"
    "----------description: 'one or more types of transactions to request',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: { type: 'array',items: {type: 'string', enum: ['ext', 'int', 'token', 'nfts', 'miner', 'all'] } },\n"
    "---------},\n"
    "---------{\n"
    "----------name: 'appearances',\n"
    "----------in: 'query',\n"
    "----------description: 'show only the blocknumer.tx_id appearances of the exported transactions',\n"
    "----------required: false,\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: {type: 'boolean'},\n"
    "---------},\n"
    "--------],\n"
    "--------responses: {\n"
    "---------'200': {\n"
    "----------description: 'status of the scraper',\n"
    "----------content: { 'application/json': {schema: {type: 'array', items: {$ref: '#/components/schemas/response'}}} },\n"
    "---------},\n"
    "---------'400': {description: 'bad input parameter'},\n"
    "--------},\n"
    "-------},\n"
    "------},\n";

const char* STR_HTML_CODE_FOOTER =
    "-----},\n"
    "          components: {\n"
    "            schemas: {\n"
    "              response: {\n"
    "                required: ['result'],\n"
    "                type: 'object',\n"
    "                properties: {\n"
    "                  data: {\n"
    "                    type: 'object',\n"
    "                    example: [],\n"
    "                  },\n"
    "                  error: {type: 'array', example: ['error 1', 'error 2'], items: {type: 'string'}},\n"
    "                },\n"
    "              },\n"
    "            },\n"
    "          },\n"
    "        };\n"
    "        // Build a system\n"
    "        const ui = SwaggerUIBundle({\n"
    "          spec: spec,\n"
    "          dom_id: '#swagger-ui',\n"
    "          deepLinking: true,\n"
    "          presets: [SwaggerUIBundle.presets.apis, SwaggerUIStandalonePreset],\n"
    "          plugins: [SwaggerUIBundle.plugins.DownloadUrl],\n"
    "          layout: 'StandaloneLayout',\n"
    "        });\n"
    "        window.ui = ui;\n"
    "      };\n"
    "    </script>\n"
    "  </body>\n"
    "</html>\n";

// clang-format on
