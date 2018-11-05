/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"
#include "sql_database.h"

static bool json;
//---------------------------------------------------------------------------------------------------
bool processConfigs(const COptions& options) {

    CAccountWatch::registerClass();
    CIncomeStatement::registerClass();
    HIDE_FIELD(CAccountWatch, "qbis");
    HIDE_FIELD(CAccountWatch, "nodeBal");
    HIDE_FIELD(CAccountWatch, "id");
    HIDE_FIELD(CAccountWatch, "deepScan");
    HIDE_FIELD(CAccountWatch, "color");

    CMySqlServer sqlServer;
    string_q host = getGlobalConfig()->getConfigStr("mysql", "host", "");
    string_q user = getGlobalConfig()->getConfigStr("mysql", "user", "");
    string_q pw   = getGlobalConfig()->getConfigStr("mysql", "pw",   "");
    sqlServer.setConnection(host, user, pw, true);
    sqlServer.useDatabase("tokenomics_local");

    string_q path = getGlobalConfig()->getConfigStr("mysql", "path-monitors", "");
    if (!fileExists(path)) {
        cerr << "Path '" << path << "' not found. Quitting...\n";
        return false;
    }

    json = options.json;
    if (json)
        cout << "[\n";
    forEveryFileInFolder(path, visitFile, &sqlServer);
    if (json)
        cout << "  }\n]\n";

    return true;
}

//---------------------------------------------------------------------------------------------------
bool visitFile(const string_q& path, void *data) {

    if (endsWith(path, '/')) {
        return forEveryFileInFolder(path + "*", visitFile, data);

    } else {

        if (endsWith(path, "config.toml") && !contains(path, "/accts/")) {

            CMySqlServer *sqlServer = (CMySqlServer *)data;

            static bool first = true;
            if (!first) {
                if (json)
                    cout << "  },\n";
            }
            if (json)
                cout << "  {\n";
            first = false;
            CToml toml(path);
            COptions opts;
            if (!opts.loadWatches(toml))
                return false;

            string_q name  = toml.getConfigStr("settings", "name", "Name");
            string_q index = toml.getConfigStr("settings", "index", int_2_Str(date_2_Ts(Now())));
            if (json) {
                cout << "    \"name\": \"" << name << "\",\n";
                cout << "    \"index\": " << index << ",\n";
                cout << "    \"addresses\": [\n";
            }

            sqlServer->insertRows("monitor_group", "monitorGroupID, monitorGroupName", "(" + index + ",\"" + name + "\")", "monitorGroupID");
            string_q values, monValues;
            for (uint32_t i = 0 ; i < opts.watches.size() ; i++) {
                UNHIDE_FIELD(CAccountWatch, "lastBlock");
                const CAccountWatch *w = &opts.watches[i];
                if (w->lastBlock == UINT_MAX)
                    HIDE_FIELD(CAccountWatch, "lastBlock");
                ostringstream os;
                os << *w;
                if (json) {
                    cout << substitute(substitute(substitute(substitute(string_q(os.str().c_str()), "\n", ""), ",", ",\n      "), "{", "      {\n      "), "}", "\n      }");
                    if (i < opts.watches.size()-1)
                        cout << ",";
                    cout << "\n";
                }

                if (i > 0) {
                    values += ",";
                    monValues += ",";
                }
                values += w->Format("(\"[{ADDRESS}]\",\"[{NAME}]\",[{FIRSTBLOCK}])");
                monValues += w->Format("(" + index + ",\"[{ADDRESS}]\")");
            }
            sqlServer->insertRows("monitor", "monitorAddress, nickname, firstBlock", values, "nickname");
            sqlServer->insertRows("monitor_monitor_group", "monitorGroupID, monitorAddress", monValues, "monitorGroupID");

            if (json)
                cout << "    ]\n";
        }

    }

    return true;
}
