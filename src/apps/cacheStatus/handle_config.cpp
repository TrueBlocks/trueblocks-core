/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//--------------------------------------------------------------------------------
bool COptions::handle_config(ostream& os) {
    if (mode.empty()) {
        manageFields("CAccountName:firstAppearance,latestAppearance,nAppearances,sizeInBytes", false);
        GETRUNTIME_CLASS(CAccountName)->sortFieldList();
        return handle_config_get(os);
    }
    return handle_config_set(os);
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
        values.push_back(isTestMode() ? "--api Provider--"
                                      : cc->getConfigStr(g1_1.name, "apiProvider", "http://localhost:8080"));
        values.push_back(isTestMode() ? "--config Path--" : cc->getConfigStr(g1_2.name, "configPath", defFolder));
        values.push_back(isTestMode() ? "--cache Path--"
                                      : cc->getConfigStr(g1_2.name, "cachePath", defFolder + "cache/"));
        values.push_back(isTestMode() ? "--index Path--"
                                      : cc->getConfigStr(g1_2.name, "indexPath", defFolder + "cache/addr_index/"));

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
        string_q v2 = (isTestMode() ? "--write Txs--" : cc->getConfigStr(g1.name, "write_txs", "true"));
        string_q v3 = (isTestMode() ? "--write Traces--" : cc->getConfigStr(g1.name, "write_traces", "true"));
        CConfigItem i2("write_txs", v2, "bool", "write transactions to the TrueBlocks binary cache", false, false);
        g1.keys.push_back(i2);
        CConfigItem i3("write_traces", v3, "bool", "write traces to the TrueBlocks binary cache", false, false);
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
            test1.group = "81-Custom";
            test1.address = "0x0000100001000010000100001000010000100001";
            test1.name = "TestWallet1";
            i1.named.push_back(test1);
            CAccountName test2;
            test2.group = "81-Custom";
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
    ENTER("handle_config_set");

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

        for (auto group : file.sections) {
            for (auto key : group.keys) {
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

                bool isDefaultConfigPath = (contains(path, "quickBlocks.toml") && group.name == "settings" &&
                                            key.name == "configPath" && val == configPathRelative(""));

                if (isDefaultConfigPath) {
                    toml.deleteKey("settings", "configPath");

                } else {
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

const char* STR_DISPLAY_WHEN = "[{BLOCKNUMBER}]\t[{TIMESTAMP}]\t[{DATE}]\t[{NAME}]";
const char* STR_DISPLAY_WHERE = "[{BLOCKNUMBER}]\t[{PATH}]\t[{CACHED}]";
const char* STR_TEST_DATA =
    "[\n"
    "  {\n"
    "    \"name\": \"quickBlocks.toml\",\n"
    "    \"sections\": [\n"
    "      {\n"
    "        \"section\": \"Providers\",\n"
    "        \"name\": \"settings\",\n"
    "        \"keys\": [\n"
    "          {\n"
    "            \"name\": \"rpcProvider\",\n"
    "            \"value\": \"--new rpc Provider--\",\n"
    "            \"type\": \"url\"\n"
    "          },\n"
    "          {\n"
    "            \"name\": \"apiProvider\",\n"
    "            \"value\": \"--new api Provider--\",\n"
    "            \"type\": \"url\"\n"
    "          },\n"
    "          {\n"
    "            \"name\": \"balanceProvider\",\n"
    "            \"value\": \"--new balance Prov--\",\n"
    "            \"type\": \"url\"\n"
    "          }\n"
    "        ]\n"
    "      },\n"
    "      {\n"
    "        \"section\": \"Paths\",\n"
    "        \"name\": \"settings\",\n"
    "        \"keys\": [\n"
    "          {\n"
    "            \"name\": \"configPath\",\n"
    "            \"value\": \"--new config Path--\",\n"
    "            \"type\": \"path\"\n"
    "          },\n"
    "          {\n"
    "            \"name\": \"cachePath\",\n"
    "            \"value\": \"--new cache Path--\",\n"
    "            \"type\": \"path\"\n"
    "          },\n"
    "          {\n"
    "            \"name\": \"indexPath\",\n"
    "            \"value\": \"--new index Path--\",\n"
    "            \"type\": \"path\"\n"
    "          }\n"
    "        ]\n"
    "      },\n"
    "      {\n"
    "        \"section\": \"Display Strings\",\n"
    "        \"name\": \"display_strs\",\n"
    "        \"keys\": [\n"
    "          {\n"
    "            \"name\": \"accountName\",\n"
    "            \"value\": \"--account name--\",\n"
    "            \"type\": \"display_str\"\n"
    "          },\n"
    "          {\n"
    "            \"name\": \"balancerecord\",\n"
    "            \"value\": \"--balance record--\",\n"
    "            \"type\": \"display_str\"\n"
    "          },\n"
    "          { \"name\": \"block\", \"value\": \"--block--\", \"type\": \"display_str\" },\n"
    "          {\n"
    "            \"name\": \"ethstate\",\n"
    "            \"value\": \"--eth state--\",\n"
    "            \"type\": \"display_str\"\n"
    "          },\n"
    "          {\n"
    "            \"name\": \"function\",\n"
    "            \"value\": \"--function--\",\n"
    "            \"type\": \"display_str\"\n"
    "          },\n"
    "          {\n"
    "            \"name\": \"logentry\",\n"
    "            \"value\": \"--logentry--\",\n"
    "            \"type\": \"display_str\"\n"
    "          },\n"
    "          {\n"
    "            \"name\": \"pricequote\",\n"
    "            \"value\": \"--pricequote--\",\n"
    "            \"type\": \"display_str\"\n"
    "          },\n"
    "          { \"name\": \"receipt\", \"value\": \"--receipt--\", \"type\": \"display_str\" },\n"
    "          { \"name\": \"trace\", \"value\": \"--trace--\", \"type\": \"display_str\" },\n"
    "          {\n"
    "            \"name\": \"transaction\",\n"
    "            \"value\": \"--transaction--\",\n"
    "            \"type\": \"display_str\"\n"
    "          },\n"
    "          {\n"
    "            \"name\": \"whenblock\",\n"
    "            \"value\": \"--when block--\",\n"
    "            \"type\": \"display_str\"\n"
    "          },\n"
    "          {\n"
    "            \"name\": \"whereblock\",\n"
    "            \"value\": \"--where block--\",\n"
    "            \"type\": \"display_str\"\n"
    "          }\n"
    "        ]\n"
    "      },\n"
    "      {\n"
    "        \"section\": \"APIs\",\n"
    "        \"name\": \"settings\",\n"
    "        \"keys\": [\n"
    "          {\n"
    "            \"name\": \"etherscan_key\",\n"
    "            \"value\": \"--new api_key--\",\n"
    "            \"type\": \"string\"\n"
    "          }\n"
    "        ]\n"
    "      },\n"
    "      {\n"
    "        \"name\": \"blockScrape.toml\",\n"
    "        \"sections\": [\n"
    "          {\n"
    "            \"section\": \"Scraper\",\n"
    "            \"name\": \"settings\",\n"
    "            \"keys\": [\n"
    "              {\n"
    "                \"name\": \"n_blocks\",\n"
    "                \"value\": \"--new n Blocks--\",\n"
    "                \"type\": \"number\"\n"
    "              },\n"
    "              {\n"
    "                \"name\": \"n_addr_procs\",\n"
    "                \"value\": \"--new n Addr Procs--\",\n"
    "                \"type\": \"number\"\n"
    "              },\n"
    "              {\n"
    "                \"name\": \"n_block_procs\",\n"
    "                \"value\": \"--new n Block Procs--\",\n"
    "                \"type\": \"number\"\n"
    "              }\n"
    "            ]\n"
    "          }\n"
    "        ]\n"
    "      },\n"
    "      {\n"
    "        \"name\": \"acctExport.toml\",\n"
    "        \"sections\": [\n"
    "          {\n"
    "            \"section\": \"Exporter\",\n"
    "            \"name\": \"settings\",\n"
    "            \"keys\": [\n"
    "              { \"name\": \"write_txs\", \"value\": \"true\", \"type\": \"bool\" },\n"
    "              { \"name\": \"write_traces\", \"value\": \"\", \"type\": \"bool\" }\n"
    "            ]\n"
    "          }\n"
    "        ]\n"
    "      },\n"
    "      {\n"
    "        \"name\": \"ethNames.toml\",\n"
    "        \"sections\": [\n"
    "          {\n"
    "            \"section\": \"Names\",\n"
    "            \"name\": \"custom\",\n"
    "            \"keys\": [\n"
    "              {\n"
    "                \"name\": \"list\",\n"
    "                \"type\": \"json array\",\n"
    "                \"named\": [\n"
    "                  {\n"
    "                    \"address\": \"0x0000100001000010000100001000010000100001\",\n"
    "                    \"group\": \"81-Custom\",\n"
    "                    \"name\": \"TestWallet1\"\n"
    "                  },\n"
    "                  {\n"
    "                    \"address\": \"0x0000200002000020000200002000020000200002\",\n"
    "                    \"group\": \"81-Custom\",\n"
    "                    \"name\": \"TestWallet2\"\n"
    "                  }\n"
    "                ]\n"
    "              }\n"
    "            ]\n"
    "          }\n"
    "        ]\n"
    "      }\n"
    "    ]\n"
    "  }\n"
    "]\n";
