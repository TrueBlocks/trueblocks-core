/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//--------------------------------------------------------------------------------
void COptions::handle_config(ostream& os) {
    if (mode.empty()) {  // it's a get
        handle_config_get(os);
    } else {
        handle_config_put(os);
    }
}

//--------------------------------------------------------------------------------
void COptions::handle_config_get(ostream& os) {
    CConfiguration config;

    const CToml *cc_1 = getGlobalConfig();
    CConfigFile  f_1("quickBlocks.toml");
    CConfigGroup g1_1("settings");
    string_q     v1_1 = (isTestMode() ? "--rpcProvider--" : cc_1->getConfigStr(g1_1.name, "rpcProvider",     "http://localhost:8545"));
    string_q     v2_1 = (isTestMode() ? "--apiProvider--" : cc_1->getConfigStr(g1_1.name, "apiProvider",     "http://localhost:8080"));
    string_q     v3_1 = (isTestMode() ? "--balanceProv--" : cc_1->getConfigStr(g1_1.name, "balanceProvider", "http://localhost:8080"));
    string_q     v4_1 = (isTestMode() ? "--configPath--"  : cc_1->getConfigStr(g1_1.name, "configPath",      "~/.quickBlocks/"));
    string_q     v5_1 = (isTestMode() ? "--cachePath--"   : cc_1->getConfigStr(g1_1.name, "cachePath",       "~/.quickBlocks/cache/"));
    string_q     v6_1 = (isTestMode() ? "--indexPath--"   : cc_1->getConfigStr(g1_1.name, "indexPath",       "~/.quickBlocks/cache/addr_index/"));
    CConfigItem  i1_1("rpcProvider",     v1_1, "url",  "the Ethereum node's RPC endpoint",                 true,  false); g1_1.keys.push_back(i1_1);
    CConfigItem  i2_1("apiProvider",     v2_1, "url",  "TrueBlocks' API endpoint",                         true,  false); g1_1.keys.push_back(i2_1);
    CConfigItem  i3_1("balanceProvider", v3_1, "url",  "alternative node endpoint for account balances",   false, false); g1_1.keys.push_back(i3_1);
    CConfigItem  i4_1("configPath",      v4_1, "path", "location of config files, read only",              true,  true);  g1_1.keys.push_back(i4_1);
    CConfigItem  i5_1("cachePath",       v5_1, "path", "location of cache (on external SSD, for example)", false, false); g1_1.keys.push_back(i5_1);
    CConfigItem  i6_1("indexPath",       v6_1, "path", "location of index (on internal SSD for speed)",    false, false); g1_1.keys.push_back(i6_1);
    f_1.groups.push_back(g1_1);
    config.files.push_back(f_1);

    const CToml *cc_2 = getGlobalConfig("blockScrape");
    CConfigFile  f_2("blockScrape.toml");
    CConfigGroup g1_2("settings");
    string_q     v1_2 = (isTestMode() ? "--nBlocks--"     : cc_2->getConfigStr(g1_2.name, "nBlocks",     "2000"));
    string_q     v2_2 = (isTestMode() ? "--nAddrProcs--"  : cc_2->getConfigStr(g1_2.name, "nAddrProcs",  "20"));
    string_q     v3_2 = (isTestMode() ? "--nBlockProcs--" : cc_2->getConfigStr(g1_2.name, "nBlockProcs", "10"));
    CConfigItem  i1_2("nBlocks",     v1_2, "uint",  "number of blocks to process per invocation of blaze (> 50)",        true, false); g1_2.keys.push_back(i1_2);
    CConfigItem  i2_2("nAddrProcs",  v2_2, "uint",  "number of parallel go processes to use to process addresses (> 0)", true, false); g1_2.keys.push_back(i2_2);
    CConfigItem  i3_2("nBlockProcs", v3_2, "uint",  "number of parallel go processes to use to process blocks (> 0)",    true, false); g1_2.keys.push_back(i3_2);
    f_2.groups.push_back(g1_2);
    config.files.push_back(f_2);

    const CToml *cc_3 = getGlobalConfig("acctExport");
    CConfigFile  f_3("acctExport.toml");
    CConfigGroup g1_3("settings");
    CConfigGroup g2_3("balances");
    string_q     v1_3 = (isTestMode() ? "--writeTxs--"    : cc_3->getConfigStr(g1_3.name, "writeTxs",     "true"));
    string_q     v2_3 = (isTestMode() ? "--writeTraces--" : cc_3->getConfigStr(g1_3.name, "writeTraces",  "true"));
    string_q     v3_3 = (isTestMode() ? "--remote--"      : cc_3->getConfigStr(g2_3.name, "remote",       "false"));
    CConfigItem  i1_3("writeTxs",    v1_3, "bool", "write transactions to the TrueBlocks binary cache", false, false); g1_3.keys.push_back(i1_3);
    CConfigItem  i2_3("writeTraces", v2_3, "bool", "write traces to the TrueBlocks binary cache",       false, false); g1_3.keys.push_back(i2_3);
    CConfigItem  i3_3("remote",      v3_3, "bool", "get balances from balanceProvider",                 false, false); g2_3.keys.push_back(i3_3);
    f_3.groups.push_back(g1_3);
    f_3.groups.push_back(g2_3);
    config.files.push_back(f_3);

    const CToml *cc_4 = getGlobalConfig("ethslurp");
    CConfigFile  f_4("ethslurp.toml");
    CConfigGroup g1_4("settings");
    string_q     v1_4 = (isTestMode() ? "--api_key--" : cc_4->getConfigStr(g1_4.name, "api_key", "<not-set>"));
    CConfigItem  i1_4("api_key", v1_4, "string", "api key required to use EtherScan apis -- private data -- not shared", false, false); g1_4.keys.push_back(i1_4);
    f_4.groups.push_back(g1_4);
    config.files.push_back(f_4);

    const CToml *cc_5 = getGlobalConfig("getAccounts");
    CConfigFile  f_5("getAccounts.toml");
    CConfigGroup g1_5("custom");
    string_q x;
    x += "[";
    x += "{ \"address\": \"0x3b1613b1613b1613b1613b1613b1613b1613b161\", \"name\": \"TestWallet1\" },";
    x += "{ \"address\": \"0xb1661b1661b1661b1661b1661b1661b1661b1661\", \"name\": \"TestWallet2\" }";
    x += "]";
    string_q v1_5 = (isTestMode() ? x : cc_5->getConfigStr(g1_5.name, "list", ""));
    replaceAll(v1_5, "address :", "\"address\":");
    replaceAll(v1_5, "name :", "\"name\":");
    replaceAll(v1_5, ", scanned : true", "");
    v1_5 = substitute(v1_5, "\"", "\\\"");
    CConfigItem  i1_5("list", v1_5, "json array", "user specific list of names for addresses -- private data -- not shared", false, false); g1_5.keys.push_back(i1_5);
    f_5.groups.push_back(g1_5);
    config.files.push_back(f_5);

    return;
}

//--------------------------------------------------------------------------------
void COptions::handle_config_put(ostream& os) {
    CConfiguration config;
    string_q str = asciiFileToString("input.json");
    config.parseJson3(str);
    {
        if (true) { //isTestMode()) {
            cout << "Would have written:" << endl;
            for (auto file : config.files) {
                for (auto group : file.groups) {
                    for (auto key : group.keys) {
                        cout << "  ";
                        cout << "getGlobalConfig(\"" << file.name << "\")->";
                        cout << "setConfigStr(\"" << group.name << "\", \"" << key.name << "\", \"" << key.value << "\");" << endl;
                    }
                }
            }
        }
    }
    return;
}
