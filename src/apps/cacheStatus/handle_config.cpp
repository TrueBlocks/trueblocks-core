/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//--------------------------------------------------------------------------------
bool COptions::handle_config(ostream& os) {
    ASSERT(mode.empty());
    manageFields("CAccountName:firstAppearance,latestAppearance,nAppearances,sizeInBytes", false);
    GETRUNTIME_CLASS(CAccountName)->sortFieldList();
    return handle_config_get(os);
}

//--------------------------------------------------------------------------------
bool COptions::handle_config_get(ostream& os) {
    ENTER("handle_config_get");

    CConfiguration config;
    {
        const CToml* cc = getGlobalConfig();
        CConfigFile f("quickBlocks.toml");

        CConfigSection g1_1("Providers", "settings");
        CConfigSection g1_2("Paths", "settings");

        string_q defFolder = configPathRelative("");
        CStringArray values;
        values.push_back(isTestMode() ? "--rpc Provider--"
                                      : cc->getConfigStr(g1_1.name, "rpcProvider", "http://localhost:8545"));
        values.push_back(isTestMode() ? "--balance Prov--"
                                      : cc->getConfigStr(g1_1.name, "balanceProvider", "http://localhost:8545"));
        values.push_back(isTestMode() ? "--config Path--" : cc->getConfigStr(g1_2.name, "configPath", defFolder));
        values.push_back(isTestMode() ? "--cache Path--"
                                      : cc->getConfigStr(g1_2.name, "cachePath", defFolder + "cache/"));
        values.push_back(isTestMode() ? "--index Path--"
                                      : cc->getConfigStr(g1_2.name, "indexPath", defFolder + "unchained/"));

        size_t cnt = 0;
        CConfigItemArray items;

        items.push_back(CConfigItem("rpcProvider", substitute(values[cnt++], "\t", "\\t"), "url",
                                    "the Ethereum node's RPC endpoint", true, false));
        items.push_back(CConfigItem("balanceProvider", substitute(values[cnt++], "\t", "\\t"), "url",
                                    "alternative node endpoint for account balances", false, false));
        for (auto item : items)
            g1_1.keys.push_back(item);
        f.sections.push_back(g1_1);
        items.clear();

        items.push_back(CConfigItem("configPath", substitute(values[cnt++], "\t", "\\t"), "path",
                                    "location of config files, read only", true, true));
        items.push_back(CConfigItem("cachePath", substitute(values[cnt++], "\t", "\\t"), "path",
                                    "location of cache (on external SSD, for example)", false, false));
        items.push_back(CConfigItem("indexPath", substitute(values[cnt++], "\t", "\\t"), "path",
                                    "location of index (on internal SSD for speed)", false, false));
        for (auto item : items)
            g1_2.keys.push_back(item);
        f.sections.push_back(g1_2);

        extern const char* STR_DISPLAY_WHEN;
        extern const char* STR_DISPLAY_WHERE;

        CStringArray values2;
        CConfigItemArray items2;
        CConfigSection g2("Display Strings", "display_strs");
        cnt = 0;
        values2.push_back(isTestMode() ? "--account Name--"
                                       : cc->getConfigStr(g2.section, "", STR_DISPLAY_ACCOUNTNAME));
        values2.push_back(isTestMode() ? "--block--" : cc->getConfigStr(g2.section, "", STR_DISPLAY_BLOCK));
        values2.push_back(isTestMode() ? "--eth state--" : cc->getConfigStr(g2.section, "", STR_DISPLAY_ETHSTATE));
        values2.push_back(isTestMode() ? "--function--" : cc->getConfigStr(g2.section, "", STR_DISPLAY_FUNCTION));
        values2.push_back(isTestMode() ? "--logentry--" : cc->getConfigStr(g2.section, "", STR_DISPLAY_LOGENTRY));
        values2.push_back(isTestMode() ? "--pricequote--" : cc->getConfigStr(g2.section, "", STR_DISPLAY_PRICEQUOTE));
        values2.push_back(isTestMode() ? "--receipt--" : cc->getConfigStr(g2.section, "", STR_DISPLAY_RECEIPT));
        values2.push_back(isTestMode() ? "--trace--" : cc->getConfigStr(g2.section, "", STR_DISPLAY_TRACE));
        values2.push_back(isTestMode() ? "--transaction--" : cc->getConfigStr(g2.section, "", STR_DISPLAY_TRANSACTION));
        values2.push_back(isTestMode() ? "--when block--" : cc->getConfigStr(g2.section, "", STR_DISPLAY_WHEN));
        values2.push_back(isTestMode() ? "--where block--" : cc->getConfigStr(g2.section, "", STR_DISPLAY_WHERE));
        extern string_q convertDisplayStr(const string_q& in);
        items2.push_back(
            CConfigItem("accountName", convertDisplayStr(values2[cnt++]), "display string", "", false, false));
        items2.push_back(CConfigItem("block", convertDisplayStr(values2[cnt++]), "display string", "", false, false));
        items2.push_back(
            CConfigItem("ethstate", convertDisplayStr(values2[cnt++]), "display string", "", false, false));
        items2.push_back(
            CConfigItem("function", convertDisplayStr(values2[cnt++]), "display string", "", false, false));
        items2.push_back(
            CConfigItem("logentry", convertDisplayStr(values2[cnt++]), "display string", "", false, false));
        items2.push_back(
            CConfigItem("pricequote", convertDisplayStr(values2[cnt++]), "display string", "", false, false));
        items2.push_back(CConfigItem("receipt", convertDisplayStr(values2[cnt++]), "display string", "", false, false));
        items2.push_back(CConfigItem("trace", convertDisplayStr(values2[cnt++]), "display string", "", false, false));
        items2.push_back(
            CConfigItem("transaction", convertDisplayStr(values2[cnt++]), "display string", "", false, false));
        items2.push_back(
            CConfigItem("whenblock", convertDisplayStr(values2[cnt++]), "display string", "", false, false));
        items2.push_back(
            CConfigItem("whereblock", convertDisplayStr(values2[cnt++]), "display string", "", false, false));
        for (auto item : items2)
            g2.keys.push_back(item);
        if (verbose || isTestMode())
            f.sections.push_back(g2);

        CConfigSection g3("APIs", "settings");
        string_q v3 = (isTestMode() ? "--api_key--" : cc->getConfigStr(g3.name, "etherscan_key", "<not-set>"));
        CConfigItem i3("etherscan_key", v3, "string", "api key for the EtherScan apis -- private data -- not shared",
                       false, false);
        g3.keys.push_back(i3);
        f.sections.push_back(g3);

        config.files.push_back(f);
    }

    {
        const CToml* cc = getGlobalConfig("blockScrape");
        CConfigFile f("blockScrape.toml");
        CConfigSection g1("Scraper", "settings");
        string_q v1 = (isTestMode() ? "--n Blocks--" : cc->getConfigStr(g1.name, "n_blocks", "2000"));
        string_q v2 = (isTestMode() ? "--n Addr Procs--" : cc->getConfigStr(g1.name, "n_addr_procs", "20"));
        string_q v3 = (isTestMode() ? "--n Block Procs--" : cc->getConfigStr(g1.name, "n_block_procs", "10"));
        CConfigItem i1("n_blocks", v1, "uint", "number of blocks to process per invocation of blaze (> 50)", true,
                       false);
        g1.keys.push_back(i1);
        CConfigItem i2("n_addr_procs", v2, "uint", "number of parallel go processes to use to process addresses (> 0)",
                       true, false);
        g1.keys.push_back(i2);
        CConfigItem i3("n_block_procs", v3, "uint", "number of parallel go processes to use to process blocks (> 0)",
                       true, false);
        g1.keys.push_back(i3);
        f.sections.push_back(g1);
        config.files.push_back(f);
    }

    {
        const CToml* cc = getGlobalConfig("acctExport");
        CConfigFile f("acctExport.toml");
        CConfigSection g1("Exporter", "settings");
        string_q v2 = (isTestMode() ? "--write Txs--" : cc->getConfigStr(g1.name, "cache_txs", "true"));
        string_q v3 = (isTestMode() ? "--write Traces--" : cc->getConfigStr(g1.name, "cache_traces", "true"));
        CConfigItem i2("cache_txs", v2, "bool", "write transactions to the TrueBlocks binary cache", false, false);
        g1.keys.push_back(i2);
        CConfigItem i3("cache_traces", v3, "bool", "write traces to the TrueBlocks binary cache", false, false);
        g1.keys.push_back(i3);
        f.sections.push_back(g1);
        config.files.push_back(f);
    }

    {
        manageFields("CAccountName:firstAppearance,latestAppearance,nAppearances,sizeInBytes", false);

        const CToml* cc = getGlobalConfig("ethNames");
        CConfigFile f("ethNames.toml");
        CConfigSection g1("Names", "custom");

        CConfigItem i1("list", "", "json array",
                       "user specific list of names for addresses -- private data -- not shared", false, false);
        if (isTestMode()) {
            CAccountName test1;
            test1.tags = "81-Custom";
            test1.address = "0x0000100001000010000100001000010000100001";
            test1.name = "TestWallet1";
            i1.named.push_back(test1);
            CAccountName test2;
            test2.tags = "81-Custom";
            test2.address = "0x0000200002000020000200002000020000200002";
            test2.name = "TestWallet2";
            i1.named.push_back(test2);

        } else {
            string_q customStr = cc->getConfigJson("custom", "list", "");
            CAccountName item;
            while (item.parseJson3(customStr)) {
                i1.named.push_back(item);
                item = CAccountName();
            }
        }

        g1.keys.push_back(i1);
        f.sections.push_back(g1);
        if (verbose || isTestMode())
            config.files.push_back(f);
    }

    os << config;
    EXIT_NOMSG(true);
}

//--------------------------------------------------------------------------------
string_q convertDisplayStr(const string_q& in) {
    string_q ret = in;
    replaceAll(ret, "\t", ",");
    replaceAll(ret, "[{", "");
    replaceAll(ret, "}]", "");
    return toLower(ret);
}

//--------------------------------------------------------------------------------
const char* STR_DISPLAY_WHEN = "[{BLOCKNUMBER}]\t[{TIMESTAMP}]\t[{DATE}]\t[{NAME}]";
const char* STR_DISPLAY_WHERE = "[{BLOCKNUMBER}]\t[{PATH}]\t[{CACHED}]";
