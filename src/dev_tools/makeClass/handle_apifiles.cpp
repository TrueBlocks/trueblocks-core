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

extern const char* STR_PATH_YAML;
extern const char* STR_PARAM_YAML;
extern const char* STR_PATH_HTML;
extern const char* STR_PARAM_HTML;
//---------------------------------------------------------------------------------------------------
void COptions::writeOpenApiFile(void) {
    if (!openapi)
        return;

    LOG_INFO(cYellow, "handling openapi file...", cOff);
    counter = CCounter();  // reset

    CCommands commands;
    options_2_Commands(commands);

    CCommandOptionArray endpointArray;
    forEveryLineInAsciiFile("../src/cmd-line-endpoints.csv", parseEndpoints, &endpointArray);

    for (auto ep : endpointArray) {
        string_q descr;
        CCommandOptionArray params;
        select_commands(optionArray, ep.api_route, params, descr);

        chifraCmdStream << ep.toChifraCmd() << endl;
        chifraHelpStream << ep.toChifraHelp() << endl;
        pairMapStream << ep.toPairMap() << endl;
        apiTagStream << ep.toApiTag();
        htmlTagStream << ep.toHtmlTag();
        goCallStream << ep.toGoCall();
        goRouteStream << ep.toGoRoute();

        if (ep.api_route != "serve" && ep.api_route != "init" && ep.api_route != "explore") {
            if (ep.api_route.empty())
                continue;
            replaceAll(ep.group, " ", "");
            apiPathStream << getApiPath(ep.group, ep.api_route, params, descr);
            htmlPathStream << getHtmlPath(ep.group, ep.api_route, params, descr);
        }
        counter.cmdCount += params.size();
        counter.routeCount++;
    }

    writeCode("../docs/api.html");
    writeCode("../docs/openapi.yaml");
    writeCode("../src/go-apps/flame/cmd/routes.go");
    writeCode("../src/apps/chifra/options.cpp");
    writeCode("../src/libs/utillib/options_base.cpp");

    if (test) {
        counter.routeCount = 0;
        LOG_WARN("Testing only - openapi file not written");
    }
    LOG_INFO(cYellow, "makeClass --openapi", cOff, " processed ", counter.routeCount, "/", counter.cmdCount,
             " routes/cmds ", " (changed ", counter.nProcessed, ").", string_q(40, ' '));
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

//---------------------------------------------------------------------------------------------------
string_q getApiPath(const string_q& group, const string_q& cmd, const CCommandOptionArray& params,
                    const string_q& descr) {
    string_q ret = STR_PATH_YAML;
    replace(ret, "[{TAGS}]", group);
    replace(ret, "[{PATH}]", cmd);
    ostringstream paramStream;
    for (auto param : params) {
        if (param.command.empty())
            continue;
        string_q yp = STR_PARAM_YAML;
        replace(yp, "[{NAME}]", param.command);
        replace(yp, "[{DESCR}]", param.swagger_descr);
        replace(yp, "[{REQ}]", param.is_required ? "true" : "false");
        replace(yp, "[{TYPE}]", getTypeStr(param, "          ", ""));
        paramStream << yp << endl;
    }
    if (!paramStream.str().empty()) {
        replace(ret, "[{PARAMS}]", paramStream.str());
    } else {
        replace(ret, "      parameters:\n[{PARAMS}]", "");
    }
    replace(ret, "[{SUMMARY}]", descr);
    replace(ret, "[{DESCR}]", descr);
    replace(ret, "[{ID}]", toLower(group + "-" + cmd));
    return ret;
}

//---------------------------------------------------------------------------------------------------
string_q getHtmlPath(const string_q& group, const string_q& cmd, const CCommandOptionArray& params,
                     const string_q& descr) {
    string_q ret = STR_PATH_HTML;
    replace(ret, "[{TAGS}]", group);
    replace(ret, "[{PATH}]", cmd);
    ostringstream paramStream;
    for (auto param : params) {
        if (param.command.empty())
            continue;
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
    }
    if (!paramStream.str().empty()) {
        replace(ret, "[{PARAMS}]", paramStream.str());
    } else {
        replace(ret, "      parameters:\n[{PARAMS}]", "");
    }
    string_q d = substitute(substitute(descr, "'", "\\'"), "\n         ", "");
    replace(ret, "[{SUMMARY}]", d);
    replace(ret, "[{DESCR}]", d);
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
void select_commands(const CCommandOptionArray& optionsArray, const string_q& route, CCommandOptionArray& cmds,
                     string_q& descr) {
    for (auto option : optionsArray) {
        if (option.api_route == route) {
            if (option.option_type == "description" && descr.empty()) {
                descr = option.swagger_descr;
            } else if (option.option_type == "note" || option.option_type == "error") {
                // do nothing
            } else if (option.option_type != "deprecated") {
                cmds.push_back(option);
            }
        }
    }
}

static CStringArray header;
//---------------------------------------------------------------------------------------------------
bool parseEndpoints(const char* str, void* data) {
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
    "            '/[{PATH}]': {\n"
    "              get: {\n"
    "                tags: ['[{TAGS}]'],\n"
    "                summary: '[{SUMMARY}]',\n"
    "                description: '[{DESCR}]',\n"
    "                operationId: '[{ID}]',\n"
    "                parameters: [\n[{PARAMS}]"
    "                ],\n"
    "                responses: {\n"
    "                  '200': {\n"
    "                    description: 'status of the scraper',\n"
    "                    content: { 'application/json': {schema: {type: 'array', items: {$ref: "
    "'#/components/schemas/response'}}} }, },\n"
    "                  '400': {description: 'bad input parameter'},\n"
    "                },\n"
    "              },\n"
    "            },\n";

//---------------------------------------------------------------------------------------------------
const char* STR_PARAM_HTML =
    "                  {\n"
    "                    name: '[{NAME}]',\n"
    "                    in: 'query',\n"
    "                    description: '[{DESCR}]',\n"
    "                    required: [{REQ}],\n"
    "                    style: 'form',\n"
    "                    explode: true,\n"
    "                    schema: [{TYPE}],\n"
    "                  },";
