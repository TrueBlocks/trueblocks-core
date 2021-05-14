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

extern const char* STR_TAG_YAML;
extern const char* STR_PATH_YAML;
extern const char* STR_PARAM_YAML;
extern const char* STR_CODE_HEADER_HTML;
extern const char* STR_CODE_FOOTER_HTML;
extern const char* STR_TAG_HTML;
extern const char* STR_PATH_HTML;
extern const char* STR_PARAM_HTML;
extern string_q getTypeStr(const CCommandOption& opt, const string_q& lead, const string_q& trail);

//---------------------------------------------------------------------------------------------------
extern bool getGroups(const char* str, void* data);

extern string_q getHtmlPath(const string_q& group, const string_q& cmd, const CCommandOptionArray& params,
                            const CCommandOptionArray& descr, CCounter& counter);
extern string_q getApiPath(const string_q& group, const string_q& cmd, const CCommandOptionArray& params,
                           const CCommandOptionArray& descr, CCounter& counter);
extern string_q getGoRoute(const string_q& goFunc, const string_q& cmd);
extern string_q getGoCall(const string_q& goFunc, const string_q& cmd, const string_q& tool);
extern void select_commands(const CCommandOptionArray& optionArray, const string_q& cmd, CCommandOptionArray& cmds,
                            CCommandOptionArray& notes, CCommandOptionArray& errors, CCommandOptionArray& descr);
//---------------------------------------------------------------------------------------------------
void COptions::writeOpenApiFile(void) {
    if (!openapi)
        return;

    LOG_INFO(cYellow, "handling openapi file...", cOff);
    counter = CCounter();  // reset

    CCommands commands;
    options_2_Commands(commands);

    CCommandOptionArray endpointArray;
    forEveryLineInAsciiFile("../src/cmd-line-endpoints.csv", getGroups, &endpointArray);

    ostringstream apiTagStream, htmlTagStream;
    ostringstream apiPathStream, htmlPathStream;
    for (auto ep : endpointArray) {
        if (ep.api_route.empty()) {
            cmdMapStream << "    // -- " << ep.group << endl;
            helpStream << "    \"" << toUpper(ep.group) << "|\"" << endl;

        } else {
            cmdMapStream << "    {\"" << ep.api_route << "\", \"" << ep.tool << "\"}," << endl;
            if (!ep.description.empty())
                helpStream << "    \"  " << padRight(ep.api_route, 14) << ep.description << "|\"" << endl;
        }

        if (ep.is_visible) {
            if (!contains(ep.tool, " ")) {
                pairMapStream << "    ";
                pairMapStream << "make_pair(\"" << ep.tool << "\", \"chifra " << ep.api_route << "\")," << endl;
            } else {
                pairMapStream << "    // " << ep.api_route << endl;
            }

        } else {
            if (ep.api_route.empty()) {
                pairMapStream << "    // -- " << ep.group << endl;
            } else {
                pairMapStream << "    // " << ep.api_route << endl;
            }
        }

        if (ep.tool.empty()) {
            string_q tag = STR_TAG_YAML;
            replace(tag, "[{NAME}]", substitute(ep.group, " ", ""));
            replace(tag, "[{DESCR}]", ep.description);
            apiTagStream << tag;

            tag = STR_TAG_HTML;
            replace(tag, "[{NAME}]", substitute(ep.group, " ", ""));
            replace(tag, "[{DESCR}]", ep.description);
            htmlTagStream << tag;
        }

        string_q group = substitute(ep.group, " ", "");
        string_q cmd = ep.api_route;
        if (cmd.empty())
            continue;
        CCommandOptionArray params, unused1, unused2, descr;
        select_commands(optionArray, cmd, params, unused1, unused2, descr);

        if (cmd != "serve" && cmd != "init" && cmd != "explore") {
            string_q goFunc = group + toProper(cmd);
            string_q tTool = descr.size() ? descr[0].tool : "";

            // Create the route handlers for the flame server
            goCallStream << getGoCall(goFunc, cmd, tTool);

            // Create the routes for the flame server
            goRouteStream << getGoRoute(goFunc, cmd);

            // Code for one openapi path
            apiPathStream << getApiPath(group, cmd, params, descr, counter);

            // Code for one htmlpath
            htmlPathStream << getHtmlPath(group, cmd, params, descr, counter);
        }

        counter.routeCount++;
    }

    writeCode("../src/apps/chifra/options.cpp");
    writeCode("../src/libs/utillib/options_base.cpp");
    writeCode("../src/go-apps/flame/cmd/routes.go");

    {
        string_q existingFile = "../docs/openapi.yaml";
        string_q existingCode = asciiFileToString(existingFile);
        string_q templateFile = configPath("makeClass/blank_openapi.yaml");
        string_q newCode = asciiFileToString(templateFile);
        replace(newCode, "[{PATHS}]", apiPathStream.str());
        replace(newCode, "[{TAGS}]", apiTagStream.str());
        if (existingCode != newCode) {
            stringToAsciiFile(existingFile, newCode);
            cerr << cGreen << "wrote " << newCode.size() << " bytes...to";
            cerr << existingFile << string_q(40, ' ') << cOff << endl;
            counter.nChanged++;
        }
    }

    {
        string_q existingFile = "../docs/api.html";
        string_q existingCode = asciiFileToString(existingFile);
        string_q body = htmlPathStream.str();
        string_q header = substitute(STR_CODE_HEADER_HTML, "[{TAGS}]", htmlTagStream.str());
        string_q footer = STR_CODE_FOOTER_HTML;
        string_q newCode = header + body + footer;
        for (size_t i = 12; i > 4; i--)
            replaceAll(newCode, string_q(i, '-'), string_q((i * 2), ' '));
        if (false) {  // existingCode != newCode) {
            stringToAsciiFile(existingFile, newCode);
            cerr << cGreen << "wrote " << newCode.size() << " bytes...to";
            cerr << existingFile << string_q(40, ' ') << cOff << endl;
            counter.nChanged++;
        }
    }

    if (test) {
        counter.routeCount = 0;
        LOG_WARN("Testing only - openapi file not written");
    }
    LOG_INFO(cYellow, "makeClass --openapi", cOff, " processed ", counter.routeCount, "/", counter.cmdCount,
             " routes/cmds ", " (changed ", counter.nChanged, ").", string_q(40, ' '));
}

//---------------------------------------------------------------------------------------------------
string_q getGoCall(const string_q& goFunc, const string_q& cmd, const string_q& tool) {
    ostringstream out;
    out << endl;
    out << "// " << goFunc << " help text todo" << endl;
    out << "func " << goFunc << "(w http.ResponseWriter, r *http.Request) {" << endl;
    if (!tool.empty()) {
        out << "\tCallOne(w, r, \"" << tool << "\", \"" << cmd << "\")" << endl;
    } else if (goFunc == "AccountsTags" || goFunc == "AccountsEntities") {
        out << "\tCallOne(w, r, \"ethNames\", \"" << cmd << "\")" << endl;
    } else {
        out << "\tCallOneExtra(w, r, \"chifra\", \"" << cmd << "\", \"" << cmd << "\")" << endl;
    }
    out << "}" << endl;
    return out.str();
}

//---------------------------------------------------------------------------------------------------
string_q getGoRoute(const string_q& goFunc, const string_q& cmd) {
    ostringstream out;
    out << endl;
    out << "\tRoute{" << endl;
    out << "\t\t\"" << goFunc << "\"," << endl;
    out << "\t\t\"GET\"," << endl;
    out << "\t\t\"/" << cmd << "\"," << endl;
    out << "\t\t" << goFunc << "," << endl;
    out << "\t}," << endl;
    return out.str();
}

//---------------------------------------------------------------------------------------------------
string_q getApiPath(const string_q& group, const string_q& cmd, const CCommandOptionArray& params,
                    const CCommandOptionArray& descr, CCounter& counter) {
    string_q ret = STR_PATH_YAML;
    replace(ret, "[{TAGS}]", group);
    replace(ret, "[{PATH}]", cmd);
    ostringstream paramStream;
    for (auto param : params) {
        string_q yp = STR_PARAM_YAML;
        replace(yp, "[{NAME}]", param.command);
        replace(yp, "[{DESCR}]", param.swagger_descr);
        replace(yp, "[{REQ}]", param.is_required ? "true" : "false");
        replace(yp, "[{TYPE}]", getTypeStr(param, "          ", ""));
        paramStream << yp << endl;
        counter.cmdCount++;
    }
    if (!paramStream.str().empty()) {
        replace(ret, "[{PARAMS}]", paramStream.str());
    } else {
        replace(ret, "      parameters:\n[{PARAMS}]", "");
    }
    if (descr.size()) {
        replace(ret, "[{SUMMARY}]", descr[0].swagger_descr);
        replace(ret, "[{DESCR}]", descr[0].swagger_descr);
    } else {
        replace(ret, "      summary: [{SUMMARY}]\n", "");
        replace(ret, "      description: [{DESCR}]\n", "");
    }
    replace(ret, "[{ID}]", toLower(group + "-" + cmd));
    return ret;
}

//---------------------------------------------------------------------------------------------------
string_q getHtmlPath(const string_q& group, const string_q& cmd, const CCommandOptionArray& params,
                     const CCommandOptionArray& descr, CCounter& counter) {
    string_q ret = STR_PATH_HTML;
    replace(ret, "[{TAGS}]", group);
    replace(ret, "[{PATH}]", cmd);
    ostringstream paramStream;
    for (auto param : params) {
        string_q yp = STR_PARAM_HTML;
        replace(yp, "[{NAME}]", param.command);
        string_q d = substitute(substitute(param.swagger_descr, "'", "\\'"), "\n         ", "");
        replace(yp, "[{DESCR}]", d);
        replace(yp, "[{REQ}]", param.is_required ? "true" : "false");
        string_q type = getTypeStr(param, "{", "}");
        replace(type, "type: boolean", "type: 'boolean'");
        replace(type, "type: number", "type: 'number'");
        replace(type, "type: string", "type: 'string'");
        replace(yp, "[{TYPE}]", type);
        paramStream << yp << endl;
        counter.cmdCount++;
    }
    if (!paramStream.str().empty()) {
        replace(ret, "[{PARAMS}]", paramStream.str());
    } else {
        replace(ret, "      parameters:\n[{PARAMS}]", "");
    }
    if (descr.size()) {
        string_q d = substitute(substitute(descr[0].swagger_descr, "'", "\\'"), "\n         ", "");
        replace(ret, "[{SUMMARY}]", d);
        replace(ret, "[{DESCR}]", d);
    } else {
        replace(ret, "      summary: [{SUMMARY}]\n", "");
        replace(ret, "      description: [{DESCR}]\n", "");
    }
    replace(ret, "[{ID}]", toLower(group + "-" + cmd));
    return ret;
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
void select_commands(const CCommandOptionArray& optionArray, const string_q& cmd, CCommandOptionArray& cmds,
                     CCommandOptionArray& notes, CCommandOptionArray& errors, CCommandOptionArray& descr) {
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
string_q getTypeStr(const CCommandOption& opt, const string_q& lead, const string_q& trail) {
    if (contains(opt.data_type, "list")) {
        if (trail == "}") {
            if (contains(opt.data_type, "enum")) {
                string_q e = substitute(substitute(opt.data_type, "list<", ""), ">", "");
                string_q str = substitute(substitute(substitute(e, "*", ""), "enum[", ""), "]", "");
                CStringArray opts;
                explode(opts, str, '|');
                ostringstream os;
                bool first = true;
                for (auto o : opts) {
                    if (!first)
                        os << ", ";
                    os << "'" << o << "'";
                    first = false;
                }
                string_q str_array_enum = "{ type: 'array', items: { type: 'string', enum: [";
                return str_array_enum + os.str() + "] } }";
            } else {
                return "{type: 'array', items: {type: 'string'}}";
            }
        } else {
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
                return "          type: array\n          items:\n            type: string";
            }
        }
    }

    if (contains(opt.data_type, "boolean")) {
        return lead + "type: boolean" + trail;

    } else if (contains(opt.data_type, "uint") || contains(opt.data_type, "double")) {
        return lead + "type: number" + trail;

    } else if (contains(opt.data_type, "enum")) {
        if (trail == "}") {
            string_q str = substitute(substitute(substitute(opt.data_type, "*", ""), "enum[", ""), "]", "");
            CStringArray opts;
            explode(opts, str, '|');
            ostringstream os;
            bool first = true;
            for (auto o : opts) {
                if (!first)
                    os << ", ";
                os << "'" << o << "'";
                first = false;
            }
            return lead + "type: string, enum: [" + os.str() + "] }";
        } else {
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
            string_q enum_head = lead + "type: string\n" + lead + "enum:\n" + trail;
            return enum_head + trim(os.str(), '\n');
        }
    }
    return lead + "type: string" + trail;
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
const char* STR_TAG_YAML =
    "- name: [{NAME}]\n"
    "  description: [{DESCR}]\n";

//---------------------------------------------------------------------------------------------------
const char* STR_PATH_YAML =
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
const char* STR_PARAM_YAML =
    "      - name: [{NAME}]\n"
    "        in: query\n"
    "        description: [{DESCR}]\n"
    "        required: [{REQ}]\n"
    "        style: form\n"
    "        explode: true\n"
    "        schema:\n"
    "[{TYPE}]";

//---------------------------------------------------------------------------------------------------
const char* STR_PATH_HTML =
    "------'/[{PATH}]': {\n"
    "-------get: {\n"
    "--------tags: ['[{TAGS}]'],\n"
    "--------summary: '[{SUMMARY}]',\n"
    "--------description: '[{DESCR}]',\n"
    "--------operationId: '[{ID}]',\n"
    "--------parameters: [\n[{PARAMS}]"
    "--------],\n"
    "--------responses: {\n"
    "---------'200': {\n"
    "----------description: 'status of the scraper',\n"
    "----------content: { 'application/json': {schema: {type: 'array', items: {$ref: "
    "'#/components/schemas/response'}}} }, },\n"
    "---------'400': {description: 'bad input parameter'},\n"
    "--------},\n"
    "-------},\n"
    "------},\n";

//---------------------------------------------------------------------------------------------------
const char* STR_PARAM_HTML =
    "---------{\n"
    "----------name: '[{NAME}]',\n"
    "----------in: 'query',\n"
    "----------description: '[{DESCR}]',\n"
    "----------required: [{REQ}],\n"
    "----------style: 'form',\n"
    "----------explode: true,\n"
    "----------schema: [{TYPE}],\n"
    "---------},";

//---------------------------------------------------------------------------------------------------
// clang-format off
const char* STR_CODE_HEADER_HTML =
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
    "          tags: [\n[{TAGS}]"
    "          ],\n"
    "-----paths: {\n";

const char* STR_CODE_FOOTER_HTML =
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

//---------------------------------------------------------------------------------------------------
const char* STR_TAG_HTML =
    "            {name: '[{NAME}]', description: '[{DESCR}]'},\n";

// clang-format on
