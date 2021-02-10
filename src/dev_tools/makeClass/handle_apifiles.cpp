/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "options.h"

#define routeCount fileCount
#define cmdCount nVisited
#define nChanged nProcessed

extern const char* STR_TAG_ENTRY;
extern const char* STR_PATH_ENTRY;
extern const char* STR_PATH_PARAM;
extern string_q getTypeStr(const CCommandOption& opt, const string_q& lead);
//---------------------------------------------------------------------------------------------------
void COptions::writeOpenApiFile(void) {
    if (!openapi)
        return;
    LOG_INFO(cYellow, "handling openapi file...", cOff);
    counter = CCounter();  // reset

    CCommands commands;
    options_2_Commands(commands);

    CStringArray endpoints = {
        "Accounts|Access and cache transactional data|export,list,tags,names,collections,abis,rm",
        "Admin|Control the scraper and build the index|status,scrape",
        "Data|Access and cache blockchain-related data|blocks,transactions,receipts,logs,traces,when",
        "State|Access to account and token state|state,tokens",
        "Other|Access to other and external data|quotes,slurp,where,dive"};

    ostringstream tagStream;
    ostringstream pathStream;
    for (auto ep : endpoints) {
        CStringArray parts;
        explode(parts, ep, '|');
        tagStream << substitute(substitute(STR_TAG_ENTRY, "[{NAME}]", parts[0]), "[{DESCR}]", parts[1]);

        CStringArray cmds;
        explode(cmds, parts[2], ',');
        for (auto cmd : cmds) {
            string_q entry = STR_PATH_ENTRY;
            replace(entry, "[{TAGS}]", parts[0]);
            replace(entry, "[{PATH}]", cmd);
            CCommandOptionArray params;
            CCommandOptionArray notes;
            CCommandOptionArray errors;
            CCommandOptionArray descr;
            select_commands(cmd, params, notes, errors, descr);
            if (descr.size()) {
                replace(entry, "[{SUMMARY}]", descr[0].swagger_descr);
                replace(entry, "[{DESCR}]", descr[0].swagger_descr);
            }
            replace(entry, "      summary: [{SUMMARY}]\n", "");
            replace(entry, "      description: [{DESCR}]\n", "");
            replace(entry, "[{ID}]", toLower(parts[0] + "-" + cmd));
            ostringstream paramStream;
            for (auto param : params) {
                string_q p = STR_PATH_PARAM;
                replace(p, "[{NAME}]", param.command);
                replace(p, "[{IN}]", (param.option_kind == "positional" ? "query" : "query"));
                replace(p, "[{DESCR}]", param.swagger_descr);
                replace(p, "[{REQ}]", param.is_required ? "true" : "false");
                replace(p, "[{TYPE}]", getTypeStr(param, "          "));
                counter.cmdCount++;
                paramStream << p << endl;
            }
            replace(entry, "[{PARAMS}]", paramStream.str());
            pathStream << entry;
            counter.routeCount++;
        }
    }

    string_q explFolder = "../../trueblocks-explorer/";
    string_q sourceFn = explFolder + "yaml-resolved/swagger.yaml";
    string_q destHTML = explFolder + "public/help/api.html";
    string_q templateFn = configPath("makeClass/blank_swagger.yaml");
    string_q converter = doCommand("which swag2html.py");

    if (!test &&
        fileExists(sourceFn) &&
        fileExists(destHTML) &&
        fileExists(templateFn) &&
        fileExists(converter)) {

        string_q oldCode = asciiFileToString(sourceFn);
        string_q newCode = asciiFileToString(templateFn);
        replace(newCode, "[{PATHS}]", pathStream.str());
        replace(newCode, "[{TAGS}]", tagStream.str());
        if (oldCode != newCode) {
            counter.nChanged++;
            stringToAsciiFile(sourceFn, newCode);

            ostringstream cmd;
            cmd << "python " << converter << " <" << sourceFn << " >" << destHTML << " ; date ; ls -l " << destHTML;
            doCommand(cmd.str());
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
void COptions::writeApiFile(void) {
    if (!api)
        return;

    LOG_INFO(cYellow, "handling api file...", cOff);
    counter = CCounter();  // reset

    CCommands commands;
    options_2_Commands(commands);

    ostringstream routeStream;
    bool firstRoute = true;
    routeStream << "{" << endl;
    for (auto route : commands.routes) {
        counter.routeCount++;
        if (!firstRoute)
            routeStream << "," << endl;
        routeStream << "  \"" << route.route << "\": {" << endl;
        bool firstCmd = true;
        for (auto command : route.commands) {
            if (command.option_kind != "note") {
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
            if (option.option_kind == "description") {
                descr.push_back(option);
            } else if (option.option_kind == "note") {
                notes.push_back(option);
            } else if (option.option_kind == "error") {
                errors.push_back(option);
            } else if (option.option_kind != "deprecated") {
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
            /*
             list<enum[blocks|transactions|traces|slurps|prices|all*]>
             list<enum[ext*|int|token|nfts|miner|all]>
             list<enum[index|monitors|collections|names|abis|caches|some*|all]>
             list<enum[monitors|index*|none|both]>
             list<enum[name|symbol|decimals|totalSupply|version|none|all*]>
             list<enum[none|some*|all|balance|nonce|code|storage|deployed|accttype]>
             */
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

//---------------------------------------------------------------------------------------------------
const char* STR_TAG_ENTRY =
    "- name: [{NAME}]\n"
    "  description: [{DESCR}]\n";

const char* STR_PATH_ENTRY =
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

const char* STR_PATH_PARAM =
    "      - name: [{NAME}]\n"
    "        in: [{IN}]\n"
    "        description: [{DESCR}]\n"
    "        required: [{REQ}]\n"
    "        style: form\n"
    "        explode: true\n"
    "        schema:\n"
    "[{TYPE}]";
