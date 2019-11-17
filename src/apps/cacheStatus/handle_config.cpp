/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//--------------------------------------------------------------------------------
bool COptions::handle_config(ostream& os) {
    if (mode.empty()) {
        manageFields("CAccountName:firstAppearance,latestAppearance,nRecords,sizeInBytes", false);
        GETRUNTIME_CLASS(CAccountName)->sortFieldList();
        return handle_config_get(os);
    }
    return handle_config_set(os);
}

//--------------------------------------------------------------------------------
bool COptions::handle_config_get(ostream& os) {
    ENTER8("handle_config_get");

    CConfiguration config;
    {
        const CToml* cc = getGlobalConfig();
        CConfigFile f("quickBlocks.toml");

        CConfigGroup g1_1("Providers", "settings");
        CConfigGroup g1_2("Paths", "settings");

        CStringArray values;
        values.push_back(isTestMode() ? "--rpc Provider--"
                                      : cc->getConfigStr(g1_1.name, "rpcProvider", "http://localhost:8545"));
        values.push_back(isTestMode() ? "--balance Prov--"
                                      : cc->getConfigStr(g1_1.name, "balanceProvider", "http://localhost:8545"));
        values.push_back(isTestMode() ? "--api Provider--"
                                      : cc->getConfigStr(g1_1.name, "apiProvider", "http://localhost:8080"));
        values.push_back(isTestMode() ? "--config Path--"
                                      : cc->getConfigStr(g1_2.name, "configPath", "~/.quickBlocks/"));
        values.push_back(isTestMode() ? "--cache Path--"
                                      : cc->getConfigStr(g1_2.name, "cachePath", "~/.quickBlocks/cache/"));
        values.push_back(isTestMode() ? "--index Path--"
                                      : cc->getConfigStr(g1_2.name, "indexPath", "~/.quickBlocks/cache/addr_index/"));

        size_t cnt = 0;
        CConfigItemArray items;

        items.push_back(CConfigItem("rpcProvider", substitute(values[cnt++], "\t", "\\t"), "url",
                                    "the Ethereum node's RPC endpoint", true, false));
        items.push_back(CConfigItem("balanceProvider", substitute(values[cnt++], "\t", "\\t"), "url",
                                    "alternative node endpoint for account balances", false, false));
        items.push_back(CConfigItem("apiProvider", substitute(values[cnt++], "\t", "\\t"), "url",
                                    "TrueBlocks' API endpoint", true, false));
        for (auto item : items)
            g1_1.keys.push_back(item);
        f.groups.push_back(g1_1);
        items.clear();

        items.push_back(CConfigItem("configPath", substitute(values[cnt++], "\t", "\\t"), "path",
                                    "location of config files, read only", true, true));
        items.push_back(CConfigItem("cachePath", substitute(values[cnt++], "\t", "\\t"), "path",
                                    "location of cache (on external SSD, for example)", false, false));
        items.push_back(CConfigItem("indexPath", substitute(values[cnt++], "\t", "\\t"), "path",
                                    "location of index (on internal SSD for speed)", false, false));
        for (auto item : items)
            g1_2.keys.push_back(item);
        f.groups.push_back(g1_2);

        extern const char* STR_DISPLAY_WHEN;
        extern const char* STR_DISPLAY_WHERE;

        CStringArray values2;
        CConfigItemArray items2;
        CConfigGroup g2("Display Strings", "display_strs");
        cnt = 0;
        values2.push_back(isTestMode() ? "--account Name--"
                                       : cc->getConfigStr(g2.section, "", STR_DISPLAY_ACCOUNTNAME));
        values2.push_back(isTestMode() ? "--balance record--"
                                       : cc->getConfigStr(g2.section, "", STR_DISPLAY_BALANCERECORD));
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
        items2.push_back(
            CConfigItem("balancerecord", convertDisplayStr(values2[cnt++]), "display string", "", false, false));
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
            f.groups.push_back(g2);
        config.files.push_back(f);
    }

    {
        const CToml* cc = getGlobalConfig("blockScrape");
        CConfigFile f("blockScrape.toml");
        CConfigGroup g1("Scraper", "settings");
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
        f.groups.push_back(g1);
        config.files.push_back(f);
    }

    {
        const CToml* cc = getGlobalConfig("acctExport");
        CConfigFile f("acctExport.toml");
        CConfigGroup g1("Exporter", "settings");
        string_q v1 = (isTestMode() ? "--write Blocks--" : cc->getConfigStr(g1.name, "write_blocks", "false"));
        string_q v2 = (isTestMode() ? "--write Txs--" : cc->getConfigStr(g1.name, "write_txs", "true"));
        string_q v3 = (isTestMode() ? "--write Traces--" : cc->getConfigStr(g1.name, "write_traces", "true"));
        CConfigItem i1("write_blocks", v1, "bool", "write blocks to the TrueBlocks binary cache", false, false);
        g1.keys.push_back(i1);
        CConfigItem i2("write_txs", v2, "bool", "write transactions to the TrueBlocks binary cache", false, false);
        g1.keys.push_back(i2);
        CConfigItem i3("write_traces", v3, "bool", "write traces to the TrueBlocks binary cache", false, false);
        g1.keys.push_back(i3);
        f.groups.push_back(g1);
        config.files.push_back(f);
    }

    {
        manageFields("CAccountName:firstAppearance,latestAppearance,nRecords,sizeInBytes", false);

        const CToml* cc = getGlobalConfig("ethNames");
        CConfigFile f("ethNames.toml");
        CConfigGroup g1("Names", "custom");

        CConfigItem i1("list", "", "json array",
                       "user specific list of names for addresses -- private data -- not shared", false, false);
        if (isTestMode()) {
            i1.named.push_back(
                CAccountName(CAccountName("81-Custom\t\t0x0000100001000010000100001000010000100001\tTestWallet1")));
            i1.named.push_back(
                CAccountName(CAccountName("81-Custom\t\t0x0000200002000020000200002000020000200002\tTestWallet2")));

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
        if (verbose || isTestMode())
            config.files.push_back(f);
    }

    {
        const CToml* cc = getGlobalConfig("ethslurp");
        CConfigFile f("ethslurp.toml");
        CConfigGroup g1("APIs", "settings");
        string_q v1 = (isTestMode() ? "--api_key--" : cc->getConfigStr(g1.name, "api_key", "<not-set>"));
        CConfigItem i1("etherscan", v1, "string",
                       "api key required to use EtherScan apis -- private data -- not shared", false, false);
        g1.keys.push_back(i1);
        f.groups.push_back(g1);
        config.files.push_back(f);
    }

    os << config;
    return true;
}

//--------------------------------------------------------------------------------
inline string_q getSettingsStr(void) {
    string_q path = getCachePath("tmp/settings.json");
    string_q ret = asciiFileToString(path);
    if (fileExists(path))
        ::remove(path.c_str());
    if (isTestMode()) {
        extern const char* STR_TEST_DATA;
        cerr.rdbuf(cout.rdbuf());
        colorsOff();
        ret = STR_TEST_DATA;
    }
    replace(ret, "[", "");
    replaceReverse(ret, "]", "");
    return ret;
}

//--------------------------------------------------------------------------------
bool COptions::handle_config_set(ostream& os) {
    ENTER8("handle_config_set");

    string_q newSettings = getSettingsStr();
    if (newSettings.empty())
        EXIT_USAGE("No settings given. Quitting...");
    if (isApiMode() || isTestMode())
        cerr << cGreen << newSettings << cOff << endl;

    CConfigFile file;
    while (file.parseJson3(newSettings)) {
        string_q path = configPath(file.name);
        if (isTestMode())
            path = "./tests/" + file.name;
        CToml toml(path);
        CToml orig("");
        orig.mergeFile(&toml);
        if (isTestMode()) {
            cerr << cYellow << string_q(120, '-') << cOff << endl;
            cerr << "As read: " << path << ":" << endl;
            cerr << cYellow << string_q(120, '-') << cOff << endl;
            cerr << bBlue << toml << endl;
            cerr << cYellow << string_q(120, '-') << cOff << endl;
        }

        for (auto group : file.groups) {
            for (auto key : group.keys) {
                if (file.name == "ethslurp.toml" && key.name == "etherscan")
                    key.name = "api_key";
                string_q val = key.getValueByName("value");
                if (contains(key.name, "list")) {
                    ostringstream oss;
                    oss << "[";
                    bool first = true;
                    for (auto name : key.named) {
                        if (!first)
                            oss << ", ";
                        oss << name;
                        first = false;
                    }
                    oss << "]";
                    val = substitute(substitute(oss.str(), "\n", ""), "  \"", " \"");
                    val = substitute(substitute(substitute(substitute(val, "}]", " }\n]"), "[{", "[\n{"), "{", "\t{"),
                                     "}, ", " },\n");
                }
                bool isBool = (key.type == "bool");
                bool isPath = (key.type == "path");
                if (isPath && !endsWith(val, '/'))
                    val += "/";
                cerr << "  "
                     << "toml.";
                cerr << (isBool ? "setConfigBool(" : "setConfigStr(");
                cerr << "\"" << group.name << "\", ";
                cerr << "\"" << key.name << "\", ";
                cerr << (isBool ? bool_2_Str(str_2_Bool(val)) : ("\"" + val + "\""));
                cerr << ");" << endl;
                if (key.type == "bool") {
                    toml.setConfigBool(group.name, key.name, str_2_Bool(val));
                } else {
                    toml.setConfigStr(group.name, key.name, val);
                }
            }
        }
        if (isTestMode())
            toml.setFilename(substitute(path, "/tests/", "/alteredTests/"));
        toml.writeFile();

        if (isTestMode()) {
            CToml newToml(substitute(path, "/tests/", "/alteredTests/"));
            cerr << cYellow << string_q(120, '-') << cOff << endl;
            cerr << "Would have written: " << path << ":" << endl;
            cerr << cYellow << string_q(120, '-') << cOff << endl;
            cerr << bBlue << newToml << endl;
            cerr << cYellow << string_q(120, '-') << cOff << endl;
        }

        file = CConfigFile();
    }

    return true;
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
const char* STR_TEST_DATA =
    "[{\"name\": \"quickBlocks.toml\",\"groups\": [{\"section\": \"Providers\",\"name\": \"settings\",\"keys\": "
    "[{\"name\": \"rpcProvider\",\"value\": \"--new rpc Provider--\",\"type\": \"url\"},{\"name\": "
    "\"apiProvider\",\"value\": \"--new api Provider--\",\"type\": \"url\"},{\"name\": \"balanceProvider\",\"value\": "
    "\"--new balance Prov--\",\"type\": \"url\"}]},{\"section\": \"Paths\",\"name\": \"settings\",\"keys\": "
    "[{\"name\": \"configPath\",\"value\": \"--new config Path--\",\"type\": \"path\"},{\"name\": "
    "\"cachePath\",\"value\": \"--new cache Path--\",\"type\": \"path\"},{\"name\": \"indexPath\",\"value\": \"--new "
    "index Path--\",\"type\": \"path\"}]}]},{\"section\": \"Display Strings\",\"name\": \"display_strs\",\"keys\": "
    "[{\"name\": \"accountName\",\"value\": \"--account name--\",\"type\": \"display_str\"},{\"name\": "
    "\"balancerecord\",\"value\": \"--balance record--\",\"type\": \"display_str\"},{\"name\": \"block\",\"value\": "
    "\"--block--\",\"type\": \"display_str\"},{\"name\": \"ethstate\",\"value\": \"--eth state--\",\"type\": "
    "\"display_str\"},{\"name\": \"function\",\"value\": \"--function--\",\"type\": \"display_str\"},{\"name\": "
    "\"logentry\",\"value\": \"--logentry--\",\"type\": \"display_str\"},{\"name\": \"pricequote\",\"value\": "
    "\"--pricequote--\",\"type\": \"display_str\"},{\"name\": \"receipt\",\"value\": \"--receipt--\",\"type\": "
    "\"display_str\"},{\"name\": \"trace\",\"value\": \"--trace--\",\"type\": \"display_str\"},{\"name\": "
    "\"transaction\",\"value\": \"--transaction--\",\"type\": \"display_str\"},{\"name\": \"whenblock\",\"value\": "
    "\"--when block--\",\"type\": \"display_str\"},{\"name\": \"whereblock\",\"value\": \"--where block--\",\"type\": "
    "\"display_str\"}]},{\"name\": \"blockScrape.toml\",\"groups\": [{\"section\": \"Scraper\",\"name\": "
    "\"settings\",\"keys\": [{\"name\": \"n_blocks\",\"value\": \"--new n Blocks--\",\"type\": \"number\"},{\"name\": "
    "\"n_addr_procs\",\"value\": \"--new n Addr Procs--\",\"type\": \"number\"},{\"name\": "
    "\"n_block_procs\",\"value\": \"--new n Block Procs--\",\"type\": \"number\"}]}]},{\"name\": "
    "\"acctExport.toml\",\"groups\": [{\"section\": \"Exporter\",\"name\": \"settings\",\"keys\": [{\"name\": "
    "\"write_blocks\",\"value\": \"\",\"type\": \"bool\"},{\"name\": \"write_txs\",\"value\": \"true\",\"type\": "
    "\"bool\"},{\"name\": \"write_traces\",\"value\": \"\",\"type\": \"bool\"}]}]},{\"name\": "
    "\"ethNames.toml\",\"groups\": [{\"section\": \"Names\",\"name\": \"custom\",\"keys\": [{\"name\": "
    "\"list\",\"type\": \"json array\",\"named\": [{\"address\": "
    "\"0x0000100001000010000100001000010000100001\",\"group\": \"81-Custom\",\"name\": \"TestWallet1\"},{\"address\": "
    "\"0x0000200002000020000200002000020000200002\",\"group\": \"81-Custom\",\"name\": "
    "\"TestWallet2\"}]}]}]},{\"name\": \"ethslurp.toml\",\"groups\": [{\"section\": \"APIs\",\"name\": "
    "\"settings\",\"keys\": [{\"name\": \"etherscan\",\"value\": \"--new api_key--\",\"type\": \"string\"}]}]}]";
