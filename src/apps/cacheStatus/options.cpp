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

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    // BEG_CODE_OPTIONS
    // clang-format off
    COption("modes", "", "list<enum[index|monitors|collections|names|abis|caches|some*|all]>", OPT_POSITIONAL, "the type of status info to retrieve"),  // NOLINT
    COption("details", "d", "", OPT_SWITCH, "include details about items found in monitors, slurps, abis, or price caches"),  // NOLINT
    COption("types", "t", "list<enum[blocks|txs|traces|slurps|prices|all*]>", OPT_FLAG, "for caches mode only, which type(s) of cache to report"),  // NOLINT
    COption("depth", "p", "<uint64>", OPT_HIDDEN | OPT_FLAG, "for cache mode only, number of levels deep to report"),
    COption("terse", "e", "", OPT_HIDDEN | OPT_SWITCH, "show a terse summary report"),
    COption("migrate", "m", "list<enum[test|abi_cache|block_cache|tx_cache|trace_cache|recon_cache|name_cache|slurp_cache|all]>", OPT_HIDDEN | OPT_FLAG, "either effectuate or test to see if a migration is necessary"),  // NOLINT
    COption("get_config", "g", "", OPT_HIDDEN | OPT_SWITCH, "returns JSON data of the editable configuration file items"),  // NOLINT
    COption("set_config", "s", "", OPT_HIDDEN | OPT_SWITCH, "accepts JSON in an env variable and writes it to configuration files"),  // NOLINT
    COption("test_start", "S", "<blknum>", OPT_HIDDEN | OPT_FLAG, "first block to process (inclusive -- testing only)"),
    COption("test_end", "E", "<blknum>", OPT_HIDDEN | OPT_FLAG, "last block to process (inclusive -- testing only)"),
    COption("", "", "", OPT_DESCRIPTION, "Report on the status of the TrueBlocks system."),
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

extern void loadPinMaps(CIndexStringMap& filenameMap, CIndexHashMap& bloomMap, CIndexHashMap& indexMap);
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    ENTER("parseArguments");

    if (!standardOptions(command))
        EXIT_NOMSG(false);

    // BEG_CODE_LOCAL_INIT
    CStringArray modes;
    CStringArray types;
    CStringArray migrate;
    bool get_config = false;
    bool set_config = false;
    blknum_t test_start = 0;
    blknum_t test_end = NOPOS;
    // END_CODE_LOCAL_INIT

    blknum_t latest = NOPOS;  // getBlockProgress(BP_CLIENT).client;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (arg == "-d" || arg == "--details") {
            details = true;

        } else if (startsWith(arg, "-t:") || startsWith(arg, "--types:")) {
            string_q types_tmp;
            if (!confirmEnum("types", types_tmp, arg))
                return false;
            types.push_back(types_tmp);
        } else if (arg == "-t" || arg == "--types") {
            return flag_required("types");

        } else if (startsWith(arg, "-p:") || startsWith(arg, "--depth:")) {
            if (!confirmUint("depth", depth, arg))
                return false;
        } else if (arg == "-p" || arg == "--depth") {
            return flag_required("depth");

        } else if (arg == "-r" || arg == "--report") {
            // clang-format off
            return usage("the --report option is deprecated, run the command with no options for the same result");  // NOLINT
            // clang-format on

        } else if (arg == "-e" || arg == "--terse") {
            terse = true;

        } else if (startsWith(arg, "-m:") || startsWith(arg, "--migrate:")) {
            string_q migrate_tmp;
            if (!confirmEnum("migrate", migrate_tmp, arg))
                return false;
            migrate.push_back(migrate_tmp);
        } else if (arg == "-m" || arg == "--migrate") {
            return flag_required("migrate");

        } else if (arg == "-g" || arg == "--get_config") {
            get_config = true;

        } else if (arg == "-s" || arg == "--set_config") {
            set_config = true;

        } else if (startsWith(arg, "-S:") || startsWith(arg, "--test_start:")) {
            if (!confirmBlockNum("test_start", test_start, arg, latest))
                return false;
        } else if (arg == "-S" || arg == "--test_start") {
            return flag_required("test_start");

        } else if (startsWith(arg, "-E:") || startsWith(arg, "--test_end:")) {
            if (!confirmBlockNum("test_end", test_end, arg, latest))
                return false;
        } else if (arg == "-E" || arg == "--test_end") {
            return flag_required("test_end");

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return invalid_option(arg);
            }

        } else {
            string_q modes_tmp;
            if (!confirmEnum("modes", modes_tmp, arg))
                return false;
            modes.push_back(modes_tmp);

            // END_CODE_AUTO
        }
    }

    // BEG_DEBUG_DISPLAY
    LOG_TEST_LIST("modes", modes, modes.empty());
    LOG_TEST_BOOL("details", details);
    LOG_TEST_LIST("types", types, types.empty());
    LOG_TEST("depth", depth, (depth == NOPOS));
    LOG_TEST_BOOL("terse", terse);
    LOG_TEST_LIST("migrate", migrate, migrate.empty());
    LOG_TEST_BOOL("get_config", get_config);
    LOG_TEST_BOOL("set_config", set_config);
    LOG_TEST("test_start", test_start, (test_start == 0));
    LOG_TEST("test_end", test_end, (test_end == NOPOS));
    // END_DEBUG_DISPLAY

    if (!migrate.empty()) {
        bool hasAll = false;
        bool hasTest = false;
        for (auto m : migrate) {
            if (m == "test") {
                if (migrate.size() > 1)
                    return usage("The `test` option tests all caches, do not add specific cache names.");
                hasTest = true;
            } else if (m == "all") {
                if (migrate.size() > 1)
                    return usage("The `all` option migrates all caches, do not add specific cache names.");
                hasAll = true;
            } else {
                cachePaths.push_back(substitute(m, "_cache", "s"));
            }
        }

        if (hasTest || hasAll) {
            CStringArray caches = {"abis",  "blocks", "traces",
                                   "names", "txs",    "slurps"};  // "recons", "monitors", "prices"
            cachePaths.clear();
            cachePaths = caches;
        }

        if (hasTest)
            return handle_migrate_test();
        return handle_migrate();
    }

    bool cs = false;
    for (auto m : modes)
        cs |= (m == "caches");
    if (Mocked(cs ? "caches" : "status"))
        EXIT_NOMSG(false);

    establishFolder(getCachePath("tmp/"));
    establishFolder(indexFolder_finalized);
    establishFolder(indexFolder_blooms);
    establishFolder(getCachePath("slurps/"));
    establishFolder(getCachePath("blocks/"));
    establishFolder(getCachePath("txs/"));
    establishFolder(getCachePath("traces/"));
    establishFolder(getCachePath("monitors/"));

    for (auto m : modes)
        mode += (m + "|");

    if (!isTestMode() && (test_start != 0 || test_end != NOPOS))
        return usage("--test_start and --test_end are only available during testing.");
    scanRange = make_pair(test_start, test_end);

    if (get_config && set_config)
        return usage("Please chose only one of --set_config and --get_config.");

    if (set_config) {
        mode = "set";
        isConfig = true;
    } else if (get_config) {
        isConfig = true;
    }

    if (!isConfig) {
        if (mode.empty() || contains(mode, "some"))
            mode = "index|monitors|collections|names|slurps|prices";
        if (contains(mode, "all")) {
            mode = "index|monitors|collections|names|abis|prices|caches";
            types.push_back("all");
        }
        mode = "|" + trim(mode, '|') + "|";

        if (contains(mode, "|caches")) {
            if (details && depth == NOPOS)
                depth = 0;
            if (depth != NOPOS && depth > 3)
                return usage("--depth parameter must be less than 4.");
            replaceAll(mode, "|caches", "");
            ASSERT(endsWith(mode, '|'));
            bool hasAll = false;
            for (auto t : types) {
                hasAll |= (t == "all");
                if (t != "all")
                    mode += (t + "|");
            }
            mode += (hasAll ? "blocks|txs|traces|slurps|prices|" : "");
        }
    }

    if (!details) {
        HIDE_FIELD(CMonitorCache, "items");
        HIDE_FIELD(CSlurpCache, "items");
        HIDE_FIELD(CPriceCache, "items");
        HIDE_FIELD(CCollectionCache, "items");
        HIDE_FIELD(CAbiCache, "items");
        HIDE_FIELD(CChainCache, "items");
    } else {
        CIndexStringMap unused;
        loadPinMaps(unused, bloomHashes, indexHashes);
    }
    HIDE_FIELD(CChainCache, "max_depth");

    EXIT_NOMSG(true);
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params, OPT_PREFUND);
    // Since we need prefunds, let's load the names library here
    loadNames();

    // BEG_CODE_INIT
    details = false;
    depth = NOPOS;
    terse = false;
    // END_CODE_INIT

    scanRange = make_pair(0, NOPOS);
    isConfig = false;
    mode = "";

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 256
#endif
#ifndef LOGIN_NAME_MAX
#define LOGIN_NAME_MAX 64
#endif
    char hostname[HOST_NAME_MAX + 1] = {0};
    gethostname(hostname, HOST_NAME_MAX);
    char username[LOGIN_NAME_MAX + 1] = {0};
    if (getlogin_r(username, LOGIN_NAME_MAX) != 0 || isDockerMode())
        strncpy(username, "nobody", 7);

    if (isTestMode()) {
        status.host = "--hostname-- (--username--)";
        status.rpc_provider = status.balance_provider = "--providers--";
        status.config_path = status.cache_path = status.index_path = "--paths--";
    } else {
        status.host = string_q(hostname) + " (" + username + ")";
        status.rpc_provider = getGlobalConfig()->getConfigStr("settings", "rpcProvider", "http://localhost:8545");
        status.balance_provider = getGlobalConfig()->getConfigStr("settings", "balanceProvider", status.rpc_provider);
        status.config_path = getConfigPath("");
        status.cache_path = getGlobalConfig()->getConfigStr("settings", "cachePath", getCachePath(""));
        status.index_path = getGlobalConfig()->getConfigStr("settings", "indexPath", getIndexPath(""));
    }
    if (!isNodeRunning()) {
        status.client_version = "Not running";
        status.client_ids = "Not running";
    } else {
        status.client_version = (isTestMode() ? "Client version" : getVersionFromClient());
        uint64_t ids[2];
        getNodeIds(ids[0], ids[1]);
        status.client_ids = "chainId: " + uint_2_Str(ids[0]) + " networkId: " + uint_2_Str(ids[1]);
    }
    status.trueblocks_version = getVersionStr();
    status.is_scraping = isTestMode() ? false : (isRunning("chifra scrape") || isRunning("blockScrape"));
    status.is_testing = isTestMode();
    status.is_api = isApiMode();
    status.is_docker = isDockerMode();
    status.is_archive = isArchiveNode();
    status.is_tracing = isTracingNode();
    status.has_eskey = getGlobalConfig("")->getConfigStr("settings", "etherscan_key", "<not_set>") != "<not_set>";
    status.has_pinkey =
        getGlobalConfig("blockScrape")->getConfigStr("settings", "pinata_api_key", "<not_set>") != "<not_set>";
    if (isTestMode())
        status.has_pinkey = false;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();

    CStatus::registerClass();
    CStatusTerse::registerClass();
    CCache::registerClass();
    CCollectionCache::registerClass();
    CCollectionCacheItem::registerClass();
    CIndexCache::registerClass();
    CIndexCacheItem::registerClass();
    CAbiCache::registerClass();
    CChainCache::registerClass();
    CMonitorCache::registerClass();
    CMonitorCacheItem::registerClass();
    CNameCache::registerClass();
    CSlurpCache::registerClass();
    CPriceCache::registerClass();
    CPriceCacheItem::registerClass();
    CAbiCacheItem::registerClass();
    CConfiguration::registerClass();
    CConfigFile::registerClass();
    CConfigSection::registerClass();
    CConfigItem::registerClass();

    UNHIDE_FIELD(CCacheBase, "nApps");
    UNHIDE_FIELD(CCacheBase, "sizeInBytes");
    UNHIDE_FIELD(CCacheBase, "firstApp");
    UNHIDE_FIELD(CCacheBase, "latestApp");

    minArgs = 0;

    // BEG_CODE_NOTES
    // clang-format off
    // clang-format on
    // END_CODE_NOTES

    // BEG_ERROR_STRINGS
    // END_ERROR_STRINGS
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
void loadPinMaps(CIndexStringMap& filenameMap, CIndexHashMap& bloomMap, CIndexHashMap& indexMap) {
    CPinnedChunkArray pinList;
    if (!pinlib_readManifest(pinList))
        return;

    for (auto pin : pinList) {
        blknum_t num = str_2_Uint(pin.fileName);
        filenameMap[num] = pin.fileName;
        bloomMap[num] = pin.bloomHash;
        indexMap[num] = pin.indexHash;
    }
}
