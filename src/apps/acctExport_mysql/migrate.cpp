/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
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
#include "sql_database.h"

//---------------------------------------------------------------------------------------------------
bool processMigrate(void) {

    CMySqlServer sqlServer;
    string_q host = getGlobalConfig()->getConfigStr("mysql", "host", "");
    string_q user = getGlobalConfig()->getConfigStr("mysql", "user", "");
    string_q pw   = getGlobalConfig()->getConfigStr("mysql", "pw",   "");
    sqlServer.setConnection(host, user, pw, true);

    string_q path = getGlobalConfig()->getConfigStr("mysql", "migration", "");
    if (!fileExists(path)) {
        cerr << "Path '" << path << "' not found. Quitting...\n";
        return false;
    }
    string results;
    asciiFileToString(path, results);
    string_q contents = trim(substitute(substitute(results, "\n", " "), ";", ";\n"),' ');
    while (!contents.empty()) {
        string_q line = substitute(trim(nextTokenClear(contents, '\n'),' '), "  ", " ");
        if (!startsWith(line, "#") && !line.empty()) {
            if (sqlServer.runQuery(line))
                sqlServer.forEveryRowInResult("", printSQLRow, NULL);
        }
    }
    return true;
}
