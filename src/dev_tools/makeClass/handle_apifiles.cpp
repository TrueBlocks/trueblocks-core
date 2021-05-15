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
void COptions::writeOpenApiFile(void) {
    if (!openapi)
        return;

    LOG_INFO(cYellow, "handling openapi file...", cOff);
    counter = CCounter();  // reset

    CCommandOptionArray endpointArray;
    forEveryLineInAsciiFile("../src/cmd-line-endpoints.csv", parseEndpoints, &endpointArray);

    for (auto ep : endpointArray) {
        CCommandOptionArray params;
        for (auto option : optionArray)
            if (option.api_route == ep.api_route && option.isChifraRoute())
                params.push_back(option);

        chifraCmdStream << ep.toChifraCmd() << endl;
        chifraHelpStream << ep.toChifraHelp() << endl;
        pairMapStream << ep.toPairMap() << endl;
        apiTagStream << ep.toApiTag();
        htmlTagStream << ep.toHtmlTag();
        goCallStream << ep.toGoCall();
        goRouteStream << ep.toGoRoute();
        ep.params = &params;
        apiPathStream << ep.toApiPath();
        htmlPathStream << ep.toHtmlPath();

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
