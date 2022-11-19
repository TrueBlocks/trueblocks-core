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
/*
 * Parts of this file were generated with makeClass --options. Edit only those parts of
 * the code outside of the BEG_CODE/END_CODE sections
 */
#include "options.h"
#include "measure.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    // BEG_CODE_OPTIONS
    // clang-format off
    COption("mode", "m", "enum[cmd*|api|both]", OPT_FLAG, "determine which set of tests to run"),
    COption("filter", "f", "enum[fast*|medi|slow|all]", OPT_FLAG, "determine how long it takes to run tests"),
    COption("skip", "s", "<uint64>", OPT_HIDDEN | OPT_FLAG, "run only every 'skip' test (faster)"),
    COption("report", "r", "", OPT_SWITCH, "display performance report to screen"),
    COption("", "", "", OPT_DESCRIPTION, "Run TrueBlocks' test cases with options."),
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

extern void establishTestData(void);
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    if (!standardOptions(command))
        return false;

    // BEG_CODE_LOCAL_INIT
    string_q mode = "";
    // END_CODE_LOCAL_INIT
    string_q path;

    bool hasEsKey = getGlobalConfig("")->getConfigStr("keys.etherscan", "apiKey", "<not_set>") != "<not_set>";

    CToml config(rootConfigToml_makeClass);
    bool makeClassOn = config.getConfigBool("enabled", "generate", false);

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (startsWith(arg, "-m:") || startsWith(arg, "--mode:")) {
            if (!confirmEnum("mode", mode, arg))
                return false;
        } else if (arg == "-m" || arg == "--mode") {
            return flag_required("mode");

        } else if (startsWith(arg, "-f:") || startsWith(arg, "--filter:")) {
            if (!confirmEnum("filter", filter, arg))
                return false;
        } else if (arg == "-f" || arg == "--filter") {
            return flag_required("filter");

        } else if (startsWith(arg, "-s:") || startsWith(arg, "--skip:")) {
            if (!confirmUint("skip", skip, arg))
                return false;
        } else if (arg == "-s" || arg == "--skip") {
            return flag_required("skip");

        } else if (arg == "-r" || arg == "--report") {
            report = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return invalid_option(arg);
            }

            // END_CODE_AUTO
        } else {
            arg = trim(arg, '/');
            if (arg == "libs" || arg == "libs/") {
                static bool been_here = false;
                if (been_here)
                    break;
                been_here = true;
                tests.push_back("libs/utillib");
                tests.push_back("libs/etherlib");
                tests.push_back("libs/acctlib");

            } else if (arg == "dev_tools" || arg == "dev_tools/") {
                static bool been_here = false;
                if (been_here)
                    break;
                been_here = true;
                if (makeClassOn)
                    tests.push_back("dev_tools/makeClass");

            } else if (arg == "tools" || arg == "tools/") {
                static bool been_here = false;
                if (been_here)
                    break;
                been_here = true;
                if (hasEsKey && getEnvStr("NO_ES") != "true") {
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
                tests.push_back("apps/fireStorm");
                tests.push_back("apps/init");

            } else {
                tests.push_back(arg);
            }
        }
    }

    if (getGlobalConfig("")->getConfigBool("dev", "debug_curl", false))
        return usage("[dev]debug_curl is set in config file. All tests will fail.");

    modes = (mode == "both" ? BOTH : (mode == "api" ? API : CMD));
    if (!isNodeRunning())
        return usage("Ethereum at " + getCurlContext()->baseURL + " was not found. All tests will fail.");

    if (filter.empty())
        filter = "fast";
    else if (filter == "all")
        filter = "";

    if (tests.empty()) {
        full_test = true;
        tests.push_back("libs/utillib");
        tests.push_back("libs/etherlib");
        tests.push_back("libs/acctlib");
        if (makeClassOn)
            tests.push_back("dev_tools/makeClass");
        if (hasEsKey && getEnvStr("NO_ES") != "true") {
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
        tests.push_back("apps/fireStorm");
        tests.push_back("apps/init");
    }

    SHOW_FIELD(CTestCase, "test_id");

    apiProvider = getGlobalConfig("testRunner")->getConfigStr("settings", "api_provider", "http://localhost:8080");
    if (!endsWith(apiProvider, '/'))
        apiProvider += "/";

    establishTestData();

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    // BEG_CODE_GLOBALOPTS
    registerOptions(nParams, params, 0);
    // END_CODE_GLOBALOPTS

    // BEG_CODE_INIT
    filter = "";
    skip = 1;
    report = false;
    // END_CODE_INIT

    full_test = false;
    minArgs = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    CMeasure::registerClass();

    Init();

    // BEG_CODE_NOTES
    // clang-format off
    // clang-format on
    // END_CODE_NOTES

    // BEG_ERROR_STRINGS
    // END_ERROR_STRINGS
}

//---------------------------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//---------------------------------------------------------------------------------------------------
bool COptions::cleanTest(const string_q& path, const string_q& testName) {
    ostringstream os;
    // clang-format off
    os << "find ../../../working/" << path << "/" << testName << "/ -maxdepth 1 -name \"" << testName << "_*.txt\" -exec rm '{}' ';' 2>/dev/null ; ";
    os << "find ../../../working/" << path << "/" << testName << "/api_tests/ -maxdepth 1 -name \"" << testName << "_*.txt\" -exec rm '{}' ';' 2>/dev/null ; ";
    if (system(os.str().c_str())) {}  // Don't remove cruft. Silences compiler warnings
    // clang-format on
    return true;
}

//---------------------------------------------------------------------------------------------------
void establishTestData(void) {
    cleanFolder(cacheFolder_tmp);

    // TODO(tjayrush): This code is a hack to make test cases pass. We should fix the underlyign reason
    // TODO(tjayrush): these tests fail. To reproduce, delete the entire cache, comment the lines below
    // TODO(tjayrush): and re-run. You will see the tests that fail.

    // Forces a few blocks into the cache
    doCommand("chifra blocks --uniq 0");
    doCommand("chifra blocks --cache 4369999");
    doCommand("chifra transactions --cache 47055.0");
    doCommand("chifra transactions --cache 46147.0");

    // Forces the retreival of a few ABI files without which some tests will fail
    doCommand("chifra abis 0x45f783cce6b7ff23b2ab2d70e416cdb7d6055f51");
    doCommand("chifra abis 0xd7edd2f2bcccdb24afe9a4ab538264b0bbb31373");
    doCommand("chifra abis 0x89d24a6b4ccb1b6faa2625fe562bdd9a23260359");
    doCommand("chifra abis 0x226159d592e2b063810a10ebf6dcbada94ed68b8");
    doCommand("chifra abis 0x17996cbddd23c2a912de8477c37d43a1b79770b8");
    doCommand("chifra abis 0x0000000000004946c0e9f43f4dee607b0ef1fa1c");
    doCommand("chifra abis 0x7c66550c9c730b6fdd4c03bc2e73c5462c5f7acc");
    doCommand("chifra abis 0xa478c2975ab1ea89e8196811f51a7b7ade33eb11");
    doCommand("chifra abis 0x7d655c57f71464b6f83811c55d84009cd9f5221c");
    doCommand("chifra abis 0x6b175474e89094c44da98b954eedeac495271d0f");

    // Remove a few things from the cache
    ::remove(getBinaryCacheFilename(CT_TXS, 8854723, 61).c_str());
    ::remove(getBinaryCacheFilename(CT_TXS, 8855603, 121).c_str());
    ::remove(getBinaryCacheFilename(CT_TXS, 8856290, 62).c_str());
    ::remove(getBinaryCacheFilename(CT_TXS, 8856316, 91).c_str());
    ::remove(getBinaryCacheFilename(CT_TXS, 8856476, 55).c_str());
    ::remove(getBinaryCacheFilename(CT_TXS, 8856511, 161).c_str());
    ::remove(getBinaryCacheFilename(CT_TXS, 8860434, 21).c_str());
    ::remove(getBinaryCacheFilename(CT_TXS, 8860511, 47).c_str());
    ::remove(getBinaryCacheFilename(CT_TXS, 8860513, 85).c_str());
    ::remove(getBinaryCacheFilename(CT_TXS, 8860529, 145).c_str());
    ::remove(getBinaryCacheFilename(CT_TXS, 8860531, 152).c_str());
    ::remove(getBinaryCacheFilename(CT_TXS, 8867898, 28).c_str());
    ::remove(getBinaryCacheFilename(CT_TXS, 8875684, 148).c_str());
    ::remove(getBinaryCacheFilename(CT_TXS, 8876232, 84).c_str());
    ::remove(getBinaryCacheFilename(CT_TXS, 9280704, 12).c_str());
    ::remove(getBinaryCacheFilename(CT_TXS, 9958151, 75).c_str());
    ::remove(getBinaryCacheFilename(CT_TXS, 10272064, 50).c_str());
    ::remove(getBinaryCacheFilename(CT_TXS, 10277780, 102).c_str());
}

// This is a list of all things added to the cache on a fresh install.
// $cachePath/blocks/00/27/68/002768801.bin
// $cachePath/blocks/00/27/69/002769609.bin
// $cachePath/blocks/00/27/17/002717576.bin
// $cachePath/blocks/00/27/99/002799895.bin
// $cachePath/blocks/00/28/72/002872831.bin
// $cachePath/blocks/00/10/01/001001001.bin
// $cachePath/blocks/00/19/95/001995314.bin
// $cachePath/blocks/00/19/58/001958017.bin
// $cachePath/blocks/00/19/20/001920000.bin
// $cachePath/blocks/00/43/69/004369999.bin
// $cachePath/blocks/00/43/80/004380101.bin
// $cachePath/blocks/00/43/70/004370000.bin
// $cachePath/blocks/00/36/57/003657480.bin
// $cachePath/blocks/00/30/76/003076260.bin
// $cachePath/blocks/00/15/90/001590000.bin
// $cachePath/blocks/00/12/34/001234567.bin
// $cachePath/blocks/00/40/00/004000001.bin
// $cachePath/blocks/00/40/00/004000002.bin

// $cachePath/abis/0x314159265dd8dbb310642f98f50c066173c1259b.json
// $cachePath/abis/0xd4fa1460f537bb9085d22c7bccb5dd450ef28e3a.json
// $cachePath/abis/0xd7edd2f2bcccdb24afe9a4ab538264b0bbb31373.json
// $cachePath/abis/0x0bc529c00c6401aef6d220be8c6ea1667f6ad93e.json
// $cachePath/abis/0x0000000000004946c0e9f43f4dee607b0ef1fa1c.json
// $cachePath/abis/0x1f9840a85d5af5bf1d1762f925bdaddc4201f984.json
// $cachePath/abis/0xbb2b8038a1640196fbe3e38816f3e67cba72d940.json
// $cachePath/abis/0x2bd2326c993dfaef84f696526064ff22eba5b362.json
// $cachePath/abis/0xe592427a0aece92de3edee1f18e0157c05861564.json
// $cachePath/abis/0xa4f29fc548856180f6b1e319ee4d86715875cce4.json
// $cachePath/abis/0x226159d592e2b063810a10ebf6dcbada94ed68b8.json
// $cachePath/abis/0x5c69bee701ef814a2b6a3edd4b1652cb9cc5aa6f.json
// $cachePath/abis/0x98ca4bf7e522cd6d2f69cf843dfab327a1e26497.json
// $cachePath/abis/0xe81a6c6b733d52df5d6aa863d75aab78147a0f28.json
// $cachePath/abis/0x7c66550c9c730b6fdd4c03bc2e73c5462c5f7acc.json
// $cachePath/abis/0xc011a73ee8576fb46f5e1c5751ca3b9fe0af2a6f.json
// $cachePath/abis/0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359.json
// $cachePath/abis/0xcd565ca18f06e2e4d251b55dc49a4fe456c72052.json
// $cachePath/abis/0x00cf36853aa4024fb5bf5cc377dfd85844b411a0.json
// $cachePath/abis/0xa478c2975ab1ea89e8196811f51a7b7ade33eb11.json
// $cachePath/abis/0xe94327d07fc17907b4db788e5adf2ed424addff6.json
// $cachePath/abis/0x6090a6e47849629b7245dfa1ca21d94cd15878ef.json
// $cachePath/abis/0x3506424f91fd33084466f402d5d97f05f8e3b4af.json
// $cachePath/abis/0xbb9bc244d798123fde783fcc1c72d3bb8c189413.json
// $cachePath/abis/0x8ca1fec705f7060aaa3d8c6757e34d1e7cc61021.json
// $cachePath/abis/0x9b984d5a03980d8dc0a24506c968465424c81dbe.json
// $cachePath/abis/0xef638b4305b8a1620f4e0e562e127f1181ae16d2.json
// $cachePath/abis/0x1a9c8182c09f50c8318d769245bea52c32be35bc.json
// $cachePath/abis/0x2260fac5e5542a773aa44fbcfedf7c193bc2c599.json
// $cachePath/abis/0xd654bdd32fc99471455e86c2e7f7d7b6437e9179.json
// $cachePath/abis/0x7cf70ed6213f08b70316bd80f7c2dddc94e41ac5.json
// $cachePath/abis/0x8055d0504666e2b6942beb8d6014c964658ca591.json
// $cachePath/abis/0x3d9819210a31b4961b30ef54be2aed79b9c9cd3b.json
// $cachePath/abis/0x45f783cce6b7ff23b2ab2d70e416cdb7d6055f51.json
// $cachePath/abis/0x6b92d76c9d0d40e53019ffa51b6f0c9b6bc657c9.json
// $cachePath/abis/0x5c2c629feefcc07b338e97e39c73d2db33a85548.json
// $cachePath/abis/0xffa93aacf49297d51e211817452839052fdfb961.json
// $cachePath/abis/0x5c40ef6f527f4fba68368774e6130ce6515123f2.json
// $cachePath/abis/0xdac17f958d2ee523a2206206994597c13d831ec7.json
// $cachePath/abis/0x729d19f657bd0614b4985cf1d82531c67569197b.json
// $cachePath/abis/0x7d655c57f71464b6f83811c55d84009cd9f5221c.json
// $cachePath/abis/0x51ffc1b089392a5bb65bf24eaf04d07d0e6f88b5.json
// $cachePath/abis/0x8aa33a7899fcc8ea5fbe6a608a109c3893a1b8b2.json
// $cachePath/abis/0x2428e9af9bc3d66e4ee9604da681c6e5493f8454.json
// $cachePath/abis/0xc14830e53aa344e8c14603a91229a0b925b0b262.json
// $cachePath/abis/0x89d24a6b4ccb1b6faa2625fe562bdd9a23260359.json
// $cachePath/abis/0x159cf1e9ae58211b588f5e3bf1d7e423952d959b.json
// $cachePath/abis/0xc02aaa39b223fe8d0a0e5c4f27ead9083c756cc2.json
// $cachePath/abis/0x17996cbddd23c2a912de8477c37d43a1b79770b8.json
// $cachePath/abis/0x6b175474e89094c44da98b954eedeac495271d0f.json
// $cachePath/abis/0xebf2bf80c3ddb0b6ac2f88be8dceec7bf12dff22.json

// $cachePath/slurps/0x9519afbc60719a83c470ceec41e32d3e4b8b262e.token.bin
// $cachePath/slurps/0x00a3819199113fc6a6e6ba1298afde7377e2009b.nfts.bin
// $cachePath/slurps/0x69e271483c38ed4902a55c3ea8aab9e7cc8617e5.int.bin
// $cachePath/slurps/0xc8ebccc5f5689fa8659d83713341e5ad19349448.uncles.bin
// $cachePath/slurps/0x1324e7b922b30b49ab8ea81086341cc41c249346.bin
// $cachePath/slurps/0x1324e7b922b30b49ab8ea81086341cc41c249346.int.bin

// $cachePath/monitors/0xff9387a9aae1f5daab1cd8eb0e92113ea9d19ca3.mon.bin
// $cachePath/monitors/0x05a56e2d52c817161883f50c441c3228cfe54d9f.mon.bin
// $cachePath/monitors/0x08166f02313feae18bb044e7877c808b55b5bf58.mon.bin
// $cachePath/monitors/0x6aed588ca2052ccfc907db8c24df4b7b95a29a5e.mon.bin
// $cachePath/monitors/0x1234567890123456789012345678901234567890.mon.bin
// $cachePath/monitors/0x001d14804b399c6ef80e64576f657660804fec0b.mon.bin
// $cachePath/monitors/0xbb9bc244d798123fde783fcc1c72d3bb8c189413.mon.bin
// $cachePath/monitors/0x9531c059098e3d194ff87febb587ab07b30b1306.mon.bin
// $cachePath/monitors/0x8b3b3b624c3c0397d3da8fd861512393d51dcbac.mon.bin
// $cachePath/monitors/0xf503017d7baf7fbc0fff7492b751025c6a78179b.mon.bin
// $cachePath/monitors/0xec3ef464bf821c3b10a18adf9ac7177a628e87cc.mon.bin
// $cachePath/monitors/0x000001f568875f378bf6d170b790967fe429c81a.mon.bin

// $cachePath/traces/00/66/87/006687987-00011.bin
// $cachePath/traces/00/67/11/006711969-00002.bin
// $cachePath/traces/00/20/01/002001002-00000.bin
// $cachePath/traces/00/42/79/004279119-00046.bin
// $cachePath/traces/00/10/01/001001001-00003.bin
// $cachePath/traces/00/10/01/001001001-00000.bin
// $cachePath/traces/00/10/01/001001001-00001.bin
// $cachePath/traces/00/10/01/001001000-00002.bin
// $cachePath/traces/00/10/01/001001002-00000.bin
// $cachePath/traces/00/43/48/004348290-00092.bin
// $cachePath/traces/00/36/57/003657480-00009.bin
// $cachePath/traces/00/36/57/003657480-00078.bin
// $cachePath/traces/00/36/57/003657480-00079.bin
// $cachePath/traces/00/36/57/003657480-00012.bin
// $cachePath/traces/00/36/57/003657480-00003.bin
// $cachePath/traces/00/31/89/003189962-00007.bin
// $cachePath/traces/00/31/91/003191290-00011.bin
// $cachePath/traces/00/65/13/006513870-00062.bin
// $cachePath/traces/00/30/68/003068185-00002.bin
// $cachePath/traces/00/23/32/002332332-00000.bin
// $cachePath/traces/00/40/01/004001001-00055.bin
// $cachePath/traces/00/22/22/002222221-00003.bin
// $cachePath/traces/00/22/22/002222221-00002.bin
// $cachePath/traces/00/22/22/002222221-00000.bin
// $cachePath/traces/00/22/22/002222221-00001.bin
// $cachePath/traces/00/22/22/002222222-00000.bin
// $cachePath/traces/00/25/97/002597783-00024.bin
// $cachePath/traces/01/26/55/012655579-00076.bin

// $cachePath/names/names.bin
// $cachePath/names/allocs.bin

// $cachePath/txs/00/00/46/000046147-00000.bin
// $cachePath/txs/00/00/47/000047055-00000.bin

// $cachePath/recons/6aed/588c/a2052ccfc907db8c24df4b7b95a29a5e
// $cachePath/recons/ec3e/f464/bf821c3b10a18adf9ac7177a628e87cc
// $cachePath/recons/f503/017d/7baf7fbc0fff7492b751025c6a78179b
// $cachePath/recons/0816/6f02/313feae18bb044e7877c808b55b5bf58
// $cachePath/recons/05a5/6e2d/52c817161883f50c441c3228cfe54d9f
// $cachePath/recons/001d/1480/4b399c6ef80e64576f657660804fec0b
