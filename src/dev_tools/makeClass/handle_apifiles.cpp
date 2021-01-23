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

extern void options_2_Commands(const CCommandOptionArray& optionArray, CCommands& commands);
extern const char* STR_DISPLAY_OPENAPI_YAML;
//---------------------------------------------------------------------------------------------------
void COptions::writeOpenApiFile(void) {
    if (!openapi)
        return;
    LOG_INFO(cYellow, "handling openapi file...", cOff);
    counter = CCounter();  // reset

    CCommands commands;
    options_2_Commands(optionArray, commands);

    COAInfo info;
    info.description = "Whatever";

    COAServer server;
    server.url = "http://localhost:8081";

    COpenApi apiObject;
    apiObject.openapi = "3.0.0";
    apiObject.info = info;
    apiObject.servers.push_back(server);

    //    for (auto route : commands.routes) {
    //        oaStream << route;
    //    }
    expContext().exportFmt = YAML1;
    ostringstream oaStream;
    oaStream << apiObject << endl;
    if (!test && folderExists("../../trueblocks-explorer/api/"))
        stringToAsciiFile("../../trueblocks-explorer/api/openapi.yml", oaStream.str());
    if (test) {
        counter.nProcessed = 0;
        LOG_WARN("Testing only - openapi file not written");
    }
    LOG_INFO(cYellow, "makeClass --options --openapi", cOff, " processed ", counter.nProcessed, " routes and ",
             counter.nVisited, " commands.", string_q(40, ' '));
}

//---------------------------------------------------------------------------------------------------
void COptions::writeApiFile(void) {
    if (!api)
        return;

    LOG_INFO(cYellow, "handling api file...", cOff);
    counter = CCounter();  // reset

    CCommands commands;
    options_2_Commands(optionArray, commands);

    ostringstream routeStream;
    bool firstRoute = true;
    routeStream << "{" << endl;
    for (auto route : commands.routes) {
        counter.nProcessed++;
        if (!firstRoute)
            routeStream << "," << endl;
        routeStream << "  \"" << route.route << "\": {" << endl;
        bool firstCmd = true;
        for (auto command : route.commands) {
            if (command.option_kind != "note") {
                counter.nVisited++;
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
        counter.nProcessed = 0;
        LOG_WARN("Testing only - api file not written");
    }
    LOG_INFO(cYellow, "makeClass --options --api", cOff, " processed ", counter.nProcessed, " routes and ",
             counter.nVisited, " commands.", string_q(40, ' '));
}

//---------------------------------------------------------------------------------------------------
void options_2_Commands(const CCommandOptionArray& optionArray, CCommands& commands) {
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
const char* STR_DISPLAY_OPENAPI_YAML =
    "openapi: [{OPENAPI}]\n"
    "info:\n[{INFO}]:\n"
    "servers:\n[{SERVERS}]:\n"
    "paths:\n[{PATHS}]\n"
    "components:\n[{COMPONENTS}]\n"
    "tags:\n[{TAGS}]\n";
