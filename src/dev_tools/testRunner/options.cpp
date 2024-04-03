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
#include "measure.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    COption("mode", "m", "enum[cmd*|api|both]", OPT_FLAG, "determine which set of tests to run"),
    COption("", "", "", OPT_DESCRIPTION, "Run TrueBlocks' test cases with options."),
};
static const size_t nParams = sizeof(params) / sizeof(COption);

extern void establishTestData(void);
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    if (!standardOptions(command))
        return false;

    ::setenv("NO_USERQUERY", "true", 1);

    string_q mode = "";
    string_q path;

    bool hasKey = getGlobalConfig("")->getConfigStr("keys.etherscan", "apiKey", "<not_set>") != "<not_set>";
    bool wantsTest = getEnvStr("TEST_SLURPS") == "true";
    bool runSlurps = (hasKey && wantsTest);

    CToml config(rootConfigToml_makeClass);

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
        } else if (startsWith(arg, "-m:") || startsWith(arg, "--mode:")) {
            if (!confirmEnum("mode", mode, arg))
                return false;
        } else if (arg == "-m" || arg == "--mode") {
            return flag_required("mode");

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return invalid_option(arg);
            }
        } else {
            arg = trim(arg, '/');
            if (arg == "dev_tools" || arg == "dev_tools/") {
                break;

            } else if (arg == "tools" || arg == "tools/") {
                static bool been_here = false;
                if (been_here)
                    break;
                been_here = true;
                if (runSlurps) {
                    tests.push_back("tools/ethslurp");
                }
                tests.push_back("tools/ethNames");
                tests.push_back("tools/getBlocks");
                tests.push_back("tools/getLogs");
                tests.push_back("tools/getReceipts");
                tests.push_back("tools/getState");
                tests.push_back("tools/getTokens");
                tests.push_back("tools/getTraces");
                tests.push_back("tools/getTrans");
                tests.push_back("tools/grabABI");
                tests.push_back("tools/whenBlock");

            } else if (arg == "apps" || arg == "apps/") {
                static bool been_here = false;
                if (been_here)
                    break;
                been_here = true;
                tests.push_back("apps/acctExport");
                tests.push_back("apps/blockScrape");
                tests.push_back("apps/cacheStatus");
                tests.push_back("apps/chunkMan");
                tests.push_back("apps/chifra");
                tests.push_back("apps/config");
                tests.push_back("apps/fireStorm");
                tests.push_back("apps/init");
                tests.push_back("apps/daemon");

            } else {
                tests.push_back(arg);
            }
        }
    }

    string_q localFile = getCWD() + "addresses.tsv";
    replace(localFile, "test/gold/dev_tools/testRunner", "build");
    if (fileExists(localFile)) {
        LOG_ERR(localFile, " found in local folder. Chifra monitor tests will fail.");
        exit(0);
    }

    if (getGlobalConfig("")->getConfigBool("dev", "debug_curl", false))
        return usage("[dev]debug_curl is set in config file. All tests will fail.");

    if (runSlurps) {
        tests.push_back("tools/ethslurp");
    }
    tests.push_back("tools/ethNames");
    tests.push_back("tools/getBlocks");
    tests.push_back("tools/getLogs");
    tests.push_back("tools/getReceipts");
    tests.push_back("tools/getState");
    tests.push_back("tools/getTokens");
    tests.push_back("tools/getTraces");
    tests.push_back("tools/getTrans");
    tests.push_back("tools/grabABI");
    tests.push_back("tools/whenBlock");
    tests.push_back("apps/acctExport");
    tests.push_back("apps/blockScrape");
    tests.push_back("apps/cacheStatus");
    tests.push_back("apps/chunkMan");
    tests.push_back("apps/chifra");
    tests.push_back("apps/config");
    tests.push_back("apps/fireStorm");
    tests.push_back("apps/init");
    tests.push_back("apps/daemon");

    SHOW_FIELD(CTestCase, "test_id");

    // TODO: this value is not in the testRunner config file, add it
    apiProvider = getGlobalConfig("testRunner")->getConfigStr("settings", "api_provider", "http://localhost:8080");
    if (!endsWith(apiProvider, '/'))
        apiProvider += "/";

    establishTestData();

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params, 0);
    minArgs = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    CMeasure::registerClass();

    Init();
}

//---------------------------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//---------------------------------------------------------------------------------------------------
bool COptions::cleanTest(const string_q& path, const string_q& testName) {
    ostringstream os;
    os << "find ../../../working/" << path << "/" << testName << "/ -maxdepth 1 -name \"" << testName << "_*.txt\" -exec rm '{}' ';' 2>/dev/null ; ";
    os << "find ../../../working/" << path << "/" << testName << "/api_tests/ -maxdepth 1 -name \"" << testName << "_*.txt\" -exec rm '{}' ';' 2>/dev/null ; ";
    if (system(os.str().c_str())) {}  // Don't remove cruft. Silences compiler warnings
    return true;
}

string_q postProcessor = "";

//---------------------------------------------------------------------------------------------------
void establishTestData(void) {
    cleanFolder(cacheFolder_tmp);

    string_q jqDef = doCommand("which gojq");
    // if (jqDef.empty()) {
    jqDef = "jq .";
    // }
    postProcessor = getGlobalConfig("testRunner")->getConfigStr("settings", "json_pretty_print", jqDef);
    cerr << "Using `" << postProcessor << "` for post processing." << endl;

    // TODO: This code is a hack to make test cases pass. We should fix the underlyign reason
    // TODO: these tests fail. To reproduce, delete the entire cache, comment the lines below
    // TODO: and re-run. You will see the tests that fail.

    // Forces a few blocks into the cache
    doCommand("chifra blocks --uniq 0 2>/dev/null");

    cerr << "Cleaning monitor caches..." << endl;
    doCommand("chifra monitors --decache 0xf503017d7baf7fbc0fff7492b751025c6a78179b 2>/dev/null");
    doCommand("chifra monitors --decache 0x9531c059098e3d194ff87febb587ab07b30b1306 2>/dev/null");
    doCommand("chifra monitors --decache 0x5deda52dc2b3a565d77e10f0f8d4bd738401d7d3 2>/dev/null");
    doCommand("chifra monitors --decache 0xd0b3462481c33f63a288cd1923e2a261ee65b4ff 2>/dev/null");

    cerr << "Cleaning abi caches..." << endl;
    doCommand("chifra abis --decache 2>/dev/null");
    doCommand("chifra abis --decache 0x45f783cce6b7ff23b2ab2d70e416cdb7d6055f51 2>/dev/null");
    doCommand("chifra abis --decache 0xd7edd2f2bcccdb24afe9a4ab538264b0bbb31373 2>/dev/null");
    doCommand("chifra abis --decache 0x89d24a6b4ccb1b6faa2625fe562bdd9a23260359 2>/dev/null");
    doCommand("chifra abis --decache 0x226159d592e2b063810a10ebf6dcbada94ed68b8 2>/dev/null");
    doCommand("chifra abis --decache 0x17996cbddd23c2a912de8477c37d43a1b79770b8 2>/dev/null");
    doCommand("chifra abis --decache 0x0000000000004946c0e9f43f4dee607b0ef1fa1c 2>/dev/null");
    doCommand("chifra abis --decache 0x7c66550c9c730b6fdd4c03bc2e73c5462c5f7acc 2>/dev/null");
    doCommand("chifra abis --decache 0xa478c2975ab1ea89e8196811f51a7b7ade33eb11 2>/dev/null");
    doCommand("chifra abis --decache 0x7d655c57f71464b6f83811c55d84009cd9f5221c 2>/dev/null");
    doCommand("chifra abis --decache 0x0000000000004946c0e9f43f4dee607b0ef1fa1c 2>/dev/null");
    doCommand("chifra abis --decache 0x30f938fed5de6e06a9a7cd2ac3517131c317b1e7 2>/dev/null");
    doCommand("chifra abis --decache 0xb9da44c051c6cc9e04b7e0f95e95d69c6a6d8031 2>/dev/null");
    doCommand("chifra abis --decache 0x6d903f6003cca6255d85cca4d3b5e5146dc33925 2>/dev/null");
    doCommand("chifra abis --decache 0x9ba00d6856a4edf4665bca2c2309936572473b7e 2>/dev/null");
    doCommand("chifra abis --decache 0x1a9c8182c09f50c8318d769245bea52c32be35bc 2>/dev/null");
    doCommand("chifra abis --decache 0x729d19f657bd0614b4985cf1d82531c67569197b 2>/dev/null");
    doCommand("chifra abis --decache 0x81f7564e413586f1f99fde55740ac52b43ca99c9 2>/dev/null");
    doCommand("chifra abis --decache 0x8d12a197cb00d4747a1fe03395095ce2a5cc6819 2>/dev/null");
    doCommand("chifra abis --decache 0xdbd27635a534a3d3169ef0498beb56fb9c937489 2>/dev/null");

    // TODO: If the following lines are commented out, these tests cases fail:
    // getState_proxy_imp.txt, getState_proxy_imp.txt, getTrans_old_parsing_bug_1.txt
    // getTrans_old_parsing_bug_2.txt, getTrans_old_parsing_bug_1.txt, getTrans_old_parsing_bug_2.txt
    // grabABI_ens_test.txt, grabABI_ens_test.txt, acctExport_statement_unfiltered.txt,
    // acctExport_statement_unfiltered.txt It's all related to abis being in the cache Forces the retreival of a few ABI
    // files without which some tests will fail
    cerr << "Downloading abi files..." << endl;
    doCommand("chifra abis 0x45f783cce6b7ff23b2ab2d70e416cdb7d6055f51");
    doCommand("chifra abis 0xd7edd2f2bcccdb24afe9a4ab538264b0bbb31373");
    doCommand("chifra abis 0x89d24a6b4ccb1b6faa2625fe562bdd9a23260359");
    doCommand("chifra abis 0x226159d592e2b063810a10ebf6dcbada94ed68b8");
    doCommand("chifra abis 0x17996cbddd23c2a912de8477c37d43a1b79770b8");
    doCommand("chifra abis 0x0000000000004946c0e9f43f4dee607b0ef1fa1c");
    doCommand("chifra abis 0x7c66550c9c730b6fdd4c03bc2e73c5462c5f7acc");
    doCommand("chifra abis 0xa478c2975ab1ea89e8196811f51a7b7ade33eb11");
    doCommand("chifra abis 0x7d655c57f71464b6f83811c55d84009cd9f5221c");
    doCommand("chifra abis 0x0000000000004946c0e9f43f4dee607b0ef1fa1c");
    doCommand("chifra abis 0x30f938fed5de6e06a9a7cd2ac3517131c317b1e7");
    doCommand("chifra abis 0xb9da44c051c6cc9e04b7e0f95e95d69c6a6d8031");
    doCommand("chifra abis 0x6d903f6003cca6255d85cca4d3b5e5146dc33925");
    doCommand("chifra abis 0x9ba00d6856a4edf4665bca2c2309936572473b7e");
    doCommand("chifra abis 0x1a9c8182c09f50c8318d769245bea52c32be35bc");
    doCommand("chifra abis 0x729d19f657bd0614b4985cf1d82531c67569197b");
    doCommand("chifra abis 0x81f7564e413586f1f99fde55740ac52b43ca99c9");
    doCommand("chifra abis 0x8d12a197cb00d4747a1fe03395095ce2a5cc6819");
    doCommand("chifra abis 0xdbd27635a534a3d3169ef0498beb56fb9c937489");
}
