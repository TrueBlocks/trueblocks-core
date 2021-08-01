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
#include "options.h"

#if 0
void COptions::exportTags(void) const {
    string_q contents = asciiFileToString(configPath("names/entities.csv"));
    CStringArray lines;
    explode(lines, contents, '\n');

    CEntityArray entities;
    CStringArray fields;
    for (auto line : lines) {
        if (fields.empty()) {
            explode(fields, line, ',');
        } else {
            CEntity entity;
            entity.parseCSV(fields, line);
            explode(entity.addresses, entity.addressList, '|');
            entity.push_back(entity);
        }
    }

    bool first = true;
    cout << "{\n  \"data\": [" << endl;
    for (auto entity : entities) {
        if (!first)
            cout << ",";
        cout << entity << endl;
        first = false;
    }
    cout << "] }" << endl;
}

/*
 if (tags) {
     manageFields("CAccountName:all", false);
     manageFields("CAccountName:tags", true);
     format = "[{TAGS}]";
     addr_only = false;
     types |= NAMED;
     types |= PREFUND;
     types |= CUSTOM;
 }

 // Prepare formatting
 string_q str = (format.empty() ? shortenFormat(STR_DISPLAY_ACCOUNTNAME) : format);
 if (verbose && !contains(format, "{SOURCE}"))
     str += "\t[{SOURCE}]";
 string_q meta = ", \"namePath\": \"" + (isTestMode() ? "--" : getCachePath("names/")) + "\"";

 // Display formatting
 configureDisplay("ethNames", "CAccountName", str, meta);
 if (!tags && (expContext().exportFmt == API1 || expContext().exportFmt == JSON1))
     manageFields("CAccountName:" + cleanFmt(STR_DISPLAY_ACCOUNTNAME));

 // Collect results for later display
 applyFilter();

 // Data wrangling
 if (addr_only) {
     HIDE_FIELD(CAccountName, "tags");
     HIDE_FIELD(CAccountName, "name");
     HIDE_FIELD(CAccountName, "symbol");
     HIDE_FIELD(CAccountName, "description");
     HIDE_FIELD(CAccountName, "source");
     HIDE_FIELD(CAccountName, "decimal");
 }

 return true;
 */
#endif
