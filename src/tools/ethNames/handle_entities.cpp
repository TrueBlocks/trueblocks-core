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

void COptions::exportEntities(const CStringArray& terms) {
    string_q contents = asciiFileToString(configPath("names/entities.csv"));
    CStringArray lines;
    explode(lines, contents, '\n');

    CEntityArray entityArray;
    CStringArray fields;
    for (auto line : lines) {
        if (fields.empty()) {
            explode(fields, line, ',');
        } else {
            CEntity entity;
            entity.parseCSV(fields, line);
            explode(entity.addresses, entity.addressList, '|');
            entityArray.push_back(entity);
        }
    }

    string_q str = STR_DISPLAY_ENTITY;
    configureDisplay("ethNames", "CEntity", str, "");
    if (expContext().exportFmt == API1 || expContext().exportFmt == JSON1)
        manageFields("CEntity:" + cleanFmt(STR_DISPLAY_ENTITY));

    bool first = true;
    bool isText = (expContext().exportFmt & (TXT1 | CSV1 | NONE1));
    for (auto entity : entityArray) {
        bool include = terms.size() == 0;

        if (terms.size() > 0) {
            for (auto term : terms) {
                ostringstream os;
                os << entity;
                include = include || contains(os.str(), term);
            }
        }

        if (include) {
            if (first)
                cout << exportPreamble(expContext().fmtMap["header"], entity.getRuntimeClass());
            if (isText) {
                cout << substitute(substitute(substitute(trim(entity.Format(expContext().fmtMap["format"]), '\t'),
                                                         "\n", "|"),
                                              "\"", ""),
                                   ",|", "|")
                     << endl;
            } else {
                if (!first)
                    cout << "," << endl;
                cout << "  ";
                indent();
                entity.toJson(cout);
                unindent();
            }
            first = false;
        }
    }

    cout << exportPostamble(errors, expContext().fmtMap["meta"]);
}
