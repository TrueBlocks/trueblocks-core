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

    {
        const CToml *cc = getGlobalConfig();
        CConfigFile  f("quickBlocks.toml");
        CConfigGroup g1("settings");
        CStringArray values;
        size_t cnt = 0;
        values.push_back(isTestMode() ? "--rpcProvider--" : cc->getConfigStr(g1.name, "rpcProvider",     "http://localhost:8545"));
        values.push_back(isTestMode() ? "--apiProvider--" : cc->getConfigStr(g1.name, "apiProvider",     "http://localhost:8080"));
        values.push_back(isTestMode() ? "--balanceProv--" : cc->getConfigStr(g1.name, "balanceProvider", "http://localhost:8080"));
        values.push_back(isTestMode() ? "--configPath--"  : cc->getConfigStr(g1.name, "configPath",      "~/.quickBlocks/"));
        values.push_back(isTestMode() ? "--cachePath--"   : cc->getConfigStr(g1.name, "cachePath",       "~/.quickBlocks/cache/"));
        values.push_back(isTestMode() ? "--indexPath--"   : cc->getConfigStr(g1.name, "indexPath",       "~/.quickBlocks/cache/addr_index/"));
        CConfigItemArray items;
        items.push_back(CConfigItem("rpcProvider",     substitute(values[cnt++],"\t","\\t"), "url",  "the Ethereum node's RPC endpoint",                 true,  false));
        items.push_back(CConfigItem("apiProvider",     substitute(values[cnt++],"\t","\\t"), "url",  "TrueBlocks' API endpoint",                         true,  false));
        items.push_back(CConfigItem("balanceProvider", substitute(values[cnt++],"\t","\\t"), "url",  "alternative node endpoint for account balances",   false, false));
        items.push_back(CConfigItem("configPath",      substitute(values[cnt++],"\t","\\t"), "path", "location of config files, read only",              true,  true));
        items.push_back(CConfigItem("cachePath",       substitute(values[cnt++],"\t","\\t"), "path", "location of cache (on external SSD, for example)", false, false));
        items.push_back(CConfigItem("indexPath",       substitute(values[cnt++],"\t","\\t"), "path", "location of index (on internal SSD for speed)",    false, false));
        for (auto item : items)
            g1.keys.push_back(item);
        f.groups.push_back(g1);

        extern const char* STR_DISPLAY_WHEN;
        extern const char* STR_DISPLAY_WHERE;

        CStringArray values2; CConfigItemArray items2;
        CConfigGroup g2("display_strs");cnt=0;
        values2.push_back(isTestMode() ? "--accountName--"   : cc->getConfigStr(g2.name, "", STR_DISPLAY_ACCOUNTNAME));
        values2.push_back(isTestMode() ? "--balancerecord--" : cc->getConfigStr(g2.name, "", STR_DISPLAY_BALANCERECORD));
        values2.push_back(isTestMode() ? "--block--"         : cc->getConfigStr(g2.name, "", STR_DISPLAY_BLOCK));
        values2.push_back(isTestMode() ? "--ethstate--"      : cc->getConfigStr(g2.name, "", STR_DISPLAY_ETHSTATE));
        values2.push_back(isTestMode() ? "--function--"      : cc->getConfigStr(g2.name, "", STR_DISPLAY_FUNCTION));
        values2.push_back(isTestMode() ? "--logentry--"      : cc->getConfigStr(g2.name, "", STR_DISPLAY_LOGENTRY));
        values2.push_back(isTestMode() ? "--pricequote--"    : cc->getConfigStr(g2.name, "", STR_DISPLAY_PRICEQUOTE));
        values2.push_back(isTestMode() ? "--receipt--"       : cc->getConfigStr(g2.name, "", STR_DISPLAY_RECEIPT));
        values2.push_back(isTestMode() ? "--trace--"         : cc->getConfigStr(g2.name, "", STR_DISPLAY_TRACE));
        values2.push_back(isTestMode() ? "--transaction--"   : cc->getConfigStr(g2.name, "", STR_DISPLAY_TRANSACTION));
        values2.push_back(isTestMode() ? "--whenblock--"     : cc->getConfigStr(g2.name, "", STR_DISPLAY_WHEN));
        values2.push_back(isTestMode() ? "--whereblock--"    : cc->getConfigStr(g2.name, "", STR_DISPLAY_WHERE));
extern string_q convertDisplayStr(const string_q& in);
        items2.push_back(CConfigItem("accountName",   convertDisplayStr(values2[cnt++]), "display string",  "", false,  false));
        items2.push_back(CConfigItem("balancerecord", convertDisplayStr(values2[cnt++]), "display string",  "", false,  false));
        items2.push_back(CConfigItem("block",         convertDisplayStr(values2[cnt++]), "display string",  "", false,  false));
        items2.push_back(CConfigItem("ethstate",      convertDisplayStr(values2[cnt++]), "display string",  "", false,  false));
        items2.push_back(CConfigItem("function",      convertDisplayStr(values2[cnt++]), "display string",  "", false,  false));
        items2.push_back(CConfigItem("logentry",      convertDisplayStr(values2[cnt++]), "display string",  "", false,  false));
        items2.push_back(CConfigItem("pricequote",    convertDisplayStr(values2[cnt++]), "display string",  "", false,  false));
        items2.push_back(CConfigItem("receipt",       convertDisplayStr(values2[cnt++]), "display string",  "", false,  false));
        items2.push_back(CConfigItem("trace",         convertDisplayStr(values2[cnt++]), "display string",  "", false,  false));
        items2.push_back(CConfigItem("transaction",   convertDisplayStr(values2[cnt++]), "display string",  "", false,  false));
        items2.push_back(CConfigItem("whenblock",     convertDisplayStr(values2[cnt++]), "display string",  "", false,  false));
        items2.push_back(CConfigItem("whereblock",    convertDisplayStr(values2[cnt++]), "display string",  "", false,  false));
        for (auto item : items2)
            g2.keys.push_back(item);
        f.groups.push_back(g2);
        config.files.push_back(f);
    }

    {
        const CToml *cc = getGlobalConfig("blockScrape");
        CConfigFile  f("blockScrape.toml");
        CConfigGroup g1("settings");
        string_q     v1 = (isTestMode() ? "--nBlocks--"     : cc->getConfigStr(g1.name, "nBlocks",     "2000"));
        string_q     v2 = (isTestMode() ? "--nAddrProcs--"  : cc->getConfigStr(g1.name, "nAddrProcs",  "20"));
        string_q     v3 = (isTestMode() ? "--nBlockProcs--" : cc->getConfigStr(g1.name, "nBlockProcs", "10"));
        CConfigItem  i1("nBlocks",     v1, "uint",  "number of blocks to process per invocation of blaze (> 50)",        true, false); g1.keys.push_back(i1);
        CConfigItem  i2("nAddrProcs",  v2, "uint",  "number of parallel go processes to use to process addresses (> 0)", true, false); g1.keys.push_back(i2);
        CConfigItem  i3("nBlockProcs", v3, "uint",  "number of parallel go processes to use to process blocks (> 0)",    true, false); g1.keys.push_back(i3);
        f.groups.push_back(g1);
        config.files.push_back(f);
    }

    {
        const CToml *cc = getGlobalConfig("acctExport");
        CConfigFile  f("acctExport.toml");
        CConfigGroup g1("settings");
        CConfigGroup g2("balances");
        string_q     v1 = (isTestMode() ? "--writeTxs--"    : cc->getConfigStr(g1.name, "writeTxs",     "true"));
        string_q     v2 = (isTestMode() ? "--writeTraces--" : cc->getConfigStr(g1.name, "writeTraces",  "true"));
        string_q     v3 = (isTestMode() ? "--remote--"      : cc->getConfigStr(g2.name, "remote",       "false"));
        CConfigItem  i1("writeTxs",    v1, "bool", "write transactions to the TrueBlocks binary cache", false, false); g1.keys.push_back(i1);
        CConfigItem  i2("writeTraces", v2, "bool", "write traces to the TrueBlocks binary cache",       false, false); g1.keys.push_back(i2);
        CConfigItem  i3("remote",      v3, "bool", "get balances from balanceProvider",                 false, false); g2.keys.push_back(i3);
        f.groups.push_back(g1);
        f.groups.push_back(g2);
        config.files.push_back(f);
    }

    {
        manageFields("CAccountName:firstAppearance,latestAppearance,nRecords,sizeInBytes", false);

        const CToml *cc = getGlobalConfig("getAccounts");
        CConfigFile  f("getAccounts.toml");
        CConfigGroup g1("custom");

        CConfigItem i1("list", "", "json array", "user specific list of names for addresses -- private data -- not shared", false, false);
        if (isTestMode()) {
            i1.named.push_back(CAccountName(CAccountName("81-Custom\t0x0000100001000010000100001000010000100001\tTestWallet1")));
            i1.named.push_back(CAccountName(CAccountName("81-Custom\t0x0000200002000020000200002000020000200002\tTestWallet2")));

        } else {

            string_q customStr = cc->getConfigJson("custom", "list", "");
            CAccountName item;
            while (item.parseJson3(customStr)) {
                i1.named.push_back(item);
                item = CAccountName();
            }
        }

        g1.keys.push_back(i1);
        f.groups.push_back(g1);
        config.files.push_back(f);
    }

    {
        const CToml *cc = getGlobalConfig("ethslurp");
        CConfigFile  f("ethslurp.toml");
        CConfigGroup g1("settings");
        string_q     v1 = (isTestMode() ? "--api_key--" : cc->getConfigStr(g1.name, "api_key", "<not-set>"));
        CConfigItem  i1("api_key", v1, "string", "api key required to use EtherScan apis -- private data -- not shared", false, false); g1.keys.push_back(i1);
        f.groups.push_back(g1);
        config.files.push_back(f);
    }

    os << config << endl;

    return;
}

//--------------------------------------------------------------------------------
void COptions::handle_config_put(ostream& os) {
    CConfiguration config;
    string_q str = asciiFileToString("setConfig_data2.json");
    str = trim(substitute(substitute(str, "\\\"", "\""), "\\n", "\n"), '\"');
    CApiResult result;
    result.parseJson3(str);
    cout << "Would have written:" << endl;
    for (auto file : result.data.files) {
        for (auto group : file.groups) {
            for (auto key : group.keys) {
                cout << "  ";
                cout << "getGlobalConfig(\"" << file.name << "\")->";
                cout << "setConfigStr(\"" << group.name << "\", \"" << key.name << "\", \"" << key.getValueByName("value") << "\");" << endl;
            }
        }
    }
    return;
}

//--------------------------------------------------------------------------------
string_q convertDisplayStr(const string_q& in) {
    string_q ret = in;
    replaceAll(ret, "\t", ",");
    replaceAll(ret, "[{", "");
    replaceAll(ret, "}]", "");
    return toLower(ret);
}

const char* STR_DISPLAY_WHEN = "[{BLOCKNUMBER}]\t[{TIMESTAMP}]\t[{DATE}]\t[{NAME}]";
const char* STR_DISPLAY_WHERE = "[{BLOCKNUMBER}]\t[{PATH}]\t[{CACHED}]";
