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
    COption("migrate", "m", "enum[test|all]", OPT_HIDDEN | OPT_FLAG, "either effectuate or test to see if a migration is necessary"),  // NOLINT
    COption("get_config", "g", "", OPT_HIDDEN | OPT_SWITCH, "returns JSON data of the editable configuration file items"),  // NOLINT
    COption("set_config", "s", "", OPT_HIDDEN | OPT_SWITCH, "accepts JSON in an env variable and writes it to configuration files"),  // NOLINT
    COption("first_block", "F", "<blknum>", OPT_HIDDEN | OPT_FLAG, "first block to process (inclusive -- testing only)"),  // NOLINT
    COption("last_block", "L", "<blknum>", OPT_HIDDEN | OPT_FLAG, "last block to process (inclusive -- testing only)"),
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
    string_q migrate = "";
    bool get_config = false;
    bool set_config = false;
    blknum_t first_block = 0;
    blknum_t last_block = NOPOS;
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
            if (!confirmEnum("migrate", migrate, arg))
                return false;
        } else if (arg == "-m" || arg == "--migrate") {
            return flag_required("migrate");

        } else if (arg == "-g" || arg == "--get_config") {
            get_config = true;

        } else if (arg == "-s" || arg == "--set_config") {
            set_config = true;

        } else if (startsWith(arg, "-F:") || startsWith(arg, "--first_block:")) {
            if (!confirmBlockNum("first_block", first_block, arg, latest))
                return false;
        } else if (arg == "-F" || arg == "--first_block") {
            return flag_required("first_block");

        } else if (startsWith(arg, "-L:") || startsWith(arg, "--last_block:")) {
            if (!confirmBlockNum("last_block", last_block, arg, latest))
                return false;
        } else if (arg == "-L" || arg == "--last_block") {
            return flag_required("last_block");

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

    CStringArray cachePaths = {
        cacheFolder_abis,
        cacheFolder_blocks,
        cacheFolder_monitors,
        cacheFolder_names,
        /* cacheFolder_objs, */
        cacheFolder_prices,
        cacheFolder_recons,
        cacheFolder_slurps,
        /* cacheFolder_tmp, */
        cacheFolder_traces,
        cacheFolder_txs,
    };
    if (migrate == "test") {
        return handle_migrate_test(cachePaths);
    } else if (migrate == "all") {
        return handle_migrate(cachePaths);
    } else if (!migrate.empty()) {
        return usage("Invalid migration: " + migrate);
    }

    bool cs = false;
    for (auto m : modes)
        cs |= (m == "caches");
    if (Mocked(cs ? "caches" : "status"))
        EXIT_NOMSG(false);

    if (!loadNames())
        return usage("Could not load names database.");

    establishFolder(cacheFolder_tmp);
    establishFolder(indexFolder_finalized);
    establishFolder(indexFolder_blooms);
    establishFolder(cacheFolder_slurps);
    establishFolder(cacheFolder_blocks);
    establishFolder(cacheFolder_txs);
    establishFolder(cacheFolder_traces);
    establishFolder(cacheFolder_monitors);

    for (auto m : modes)
        mode += (m + "|");

    if (!isTestMode() && (first_block != 0 || (last_block != NOPOS && last_block != 0)))
        return usage("--first_block (" + uint_2_Str(first_block) + ") and --last_block (" + uint_2_Str(last_block) +
                     ") are only available during testing.");
    scanRange = make_pair(first_block, last_block);

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
    // BEG_CODE_GLOBALOPTS
    registerOptions(nParams, params, OPT_PREFUND);
    // END_CODE_GLOBALOPTS

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
        status.rpcProvider = "--providers--";
        status.configPath = status.cachePath = status.indexPath = "--paths--";
    } else {
        status.host = string_q(hostname) + " (" + username + ")";
        // FIXME: THIS IS BROKEN
        status.rpcProvider = getRpcProvider();
        status.configPath = rootConfigs;
        status.cachePath = cacheFolder;
        status.indexPath = indexFolder;
    }
    if (!isNodeRunning()) {
        status.clientVersion = "Not running";
        status.clientIds = "Not running";
    } else {
        CMetaData meta = getMetaData();
        status.clientVersion = (isTestMode() ? "Client version" : getVersionFromClient());
        status.clientIds = "chainId: " + uint_2_Str(meta.chainId) + " networkId: " + uint_2_Str(meta.networkId);
    }
    status.trueblocksVersion = getVersionStr();
    status.isScraping = isTestMode() ? false : (isRunning("chifra scrape") || isRunning("blockScrape"));
    status.isTesting = isTestMode();
    status.isApi = isApiMode();
    status.isDocker = isDockerMode();
    status.isArchive = isArchiveNode();
    status.isTracing = isTracingNode();
    status.hasEskey = getGlobalConfig("")->getConfigStr("settings", "etherscan_key", "<not_set>") != "<not_set>";
    status.hasPinkey =
        getGlobalConfig("blockScrape")->getConfigStr("settings", "pinata_api_key", "<not_set>") != "<not_set>";
    if (isTestMode())
        status.hasPinkey = false;
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
