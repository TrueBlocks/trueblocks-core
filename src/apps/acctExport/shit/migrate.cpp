/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
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
    string_q contents;
    asciiFileToString(path, contents);
    contents = trim(substitute(substitute(contents, "\n", " "), ";", ";\n"),' ');
    while (!contents.empty()) {
        string_q line = substitute(trim(nextTokenClear(contents, '\n'),' '), "  ", " ");
        if (!startsWith(line, "#") && !line.empty()) {
            if (sqlServer.runQuery(line))
                sqlServer.forEveryRowInResult("", printSQLRow, NULL);
        }
    }
    return true;
}
