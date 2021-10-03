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
    forEveryLineInAsciiFile(endpointFile, parseCommandData, &endpointArray);

    for (auto ep : endpointArray) {
        CCommandOptionArray params;
        for (auto option : optionArray)
            if (option.api_route == ep.api_route && option.isChifraRoute(false))
                params.push_back(option);
        ep.params = &params;

        chifraCmdStream << ep.toChifraCmd() << endl;
        chifraHelpStream << ep.toChifraHelp() << endl;
        pairMapStream << ep.toPairMap() << endl;
        apiTagStream << ep.toApiTag();
        goCallStream << ep.toGoCall();
        goRouteStream << ep.toGoRoute();
        apiPathStream << ep.toApiPath(getProductions(ep));
        counter.cmdCount += params.size();
        counter.routeCount++;
    }

    writeCodeOut(this, getDocsPathContent("api/openapi.yaml"));
    writeCodeOut(this, getSourcePath("go-apps/flame/server/server_routes.go"));
    writeCodeOut(this, getSourcePath("apps/chifra/options.cpp"));
    // writeCodeOut(this, getSourcePath("libs/utillib/options_base.cpp"));

    LOG_INFO(cYellow, "makeClass --openapi", cOff, " processed ", counter.routeCount, "/", counter.cmdCount,
             " routes/cmds ", " (changed ", counter.nProcessed, ").", string_q(40, ' '));
}

namespace qblocks {
extern bool isApiRoute(const string_q& route);
}

//---------------------------------------------------------------------------------------------------
string_q COptions::getProductions(const CCommandOption& ep) {
    if (!isApiRoute(ep.api_route))
        return "";
    CStringArray productions;
    string_q descr;
    for (auto model : dataModels) {
        if (contains(model.doc_producer, ep.api_route)) {
            productions.push_back(model.doc_api);
            if (descr.empty())
                descr = model.doc_descr;
        }
    }

    string_q prods;
    for (auto p : productions) {
        prods += "$ref: \"#/components/schemas/" + p + "\"\n";
    }
    if (productions.size() > 1) {
        prods = "oneOf:\n" + prods;
        replaceAll(prods, "$ref:", "      - $ref:");
    }

    return trim(prods, '\n') + "|" + descr;
}
