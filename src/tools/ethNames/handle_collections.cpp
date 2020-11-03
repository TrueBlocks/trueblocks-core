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

void COptions::exportCollections(const CStringArray& terms) {
    string_q contents = asciiFileToString(configPath("names/collections.csv"));
    CStringArray lines;
    explode(lines, contents, '\n');

    CCollectionArray collectionArray;
    CStringArray fields;
    for (auto line : lines) {
        if (fields.empty()) {
            explode(fields, line, ',');
        } else {
            CCollection collection;
            collection.parseCSV(fields, line);
            explode(collection.addresses, collection.addressList, '|');
            collectionArray.push_back(collection);
        }
    }

    string_q str = STR_DISPLAY_COLLECTION;
    configureDisplay("ethNames", "CCollection", str, "");
    if (expContext().exportFmt == API1 || expContext().exportFmt == JSON1)
        manageFields("CCollection:" + cleanFmt(STR_DISPLAY_COLLECTION));

    bool first = true;
    bool isText = (expContext().exportFmt & (TXT1 | CSV1 | NONE1));
    for (auto collection : collectionArray) {
        bool include = terms.size() == 0;

        if (terms.size() > 0) {
            for (auto term : terms) {
                ostringstream os;
                os << collection;
                include = include || contains(os.str(), term);
            }
        }

        if (include) {
            if (first)
                cout << exportPreamble(expContext().fmtMap["header"], collection.getRuntimeClass());
            if (isText) {
                cout << substitute(substitute(substitute(trim(collection.Format(expContext().fmtMap["format"]), '\t'),
                                                         "\n", "|"),
                                              "\"", ""),
                                   ",|", "|")
                     << endl;
            } else {
                if (!first)
                    cout << "," << endl;
                cout << "  ";
                indent();
                collection.doExport(cout);
                unindent();
            }
            first = false;
        }
    }

    cout << exportPostamble(errors, expContext().fmtMap["meta"]);
}
