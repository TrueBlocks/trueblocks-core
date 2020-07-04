/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
/*
 * Parts of this file were generated with makeClass. Edit only those parts of the code
 * outside of the BEG_CODE/END_CODE sections
 */
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    // BEG_CODE_OPTIONS
    // clang-format off
    COption("modes", "", "list<enum[index|monitors|collections|names|abis|caches|some*|all]>", OPT_POSITIONAL, "the type of status info to retrieve"),  // NOLINT
    COption("details", "d", "", OPT_SWITCH, "include details about items found in monitors, slurps, abis, or price caches"),  // NOLINT
    COption("types", "t", "list<enum[blocks|transactions|traces|slurps|prices|all*]>", OPT_FLAG, "for cache mode only, which type(s) of cache to report"),  // NOLINT
    COption("depth", "p", "<uint64>", OPT_HIDDEN | OPT_FLAG, "for cache mode only, number of levels deep to report"),
    COption("report", "r", "", OPT_SWITCH, "show a summary of the current status of the blockchain and TrueBlocks scrapers"),  // NOLINT
    COption("get_config", "g", "", OPT_HIDDEN | OPT_SWITCH, "returns JSON data of the editable configuration file items"),  // NOLINT
    COption("set_config", "s", "", OPT_HIDDEN | OPT_SWITCH, "accepts JSON in an env variable and writes it to configuration files"),  // NOLINT
    COption("start", "S", "<blknum>", OPT_HIDDEN | OPT_FLAG, "first block to process (inclusive)"),
    COption("end", "E", "<blknum>", OPT_HIDDEN | OPT_FLAG, "last block to process (inclusive)"),
    COption("", "", "", OPT_DESCRIPTION, "Report on status of one or more TrueBlocks caches."),
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    ENTER("parseArguments");

    if (!standardOptions(command))
        EXIT_NOMSG(false);

    // BEG_CODE_LOCAL_INIT
    CStringArray modes;
    CStringArray types;
    bool report = false;
    bool get_config = false;
    bool set_config = false;
    // END_CODE_LOCAL_INIT

    blknum_t latest = NOPOS;  // getLatestBlock_client();

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

        } else if (startsWith(arg, "-p:") || startsWith(arg, "--depth:")) {
            if (!confirmUint("depth", depth, arg))
                return false;

        } else if (arg == "-r" || arg == "--report") {
            report = true;

        } else if (arg == "-g" || arg == "--get_config") {
            get_config = true;

        } else if (arg == "-s" || arg == "--set_config") {
            set_config = true;

        } else if (startsWith(arg, "-S:") || startsWith(arg, "--start:")) {
            if (!confirmBlockNum("start", start, arg, latest))
                return false;

        } else if (startsWith(arg, "-E:") || startsWith(arg, "--end:")) {
            if (!confirmBlockNum("end", end, arg, latest))
                return false;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {
            string_q modes_tmp;
            if (!confirmEnum("modes", modes_tmp, arg))
                return false;
            modes.push_back(modes_tmp);

            // END_CODE_AUTO
        }
    }

    // removes warning on Ubuntu 20.04
    if (report) cerr << "";

    establishFolder(getCachePath("tmp/"));
    establishFolder(getCachePath("slurps/"));
    establishFolder(getCachePath("blocks/"));
    establishFolder(getCachePath("txs/"));
    establishFolder(getCachePath("traces/"));
    establishFolder(getCachePath("monitors/"));

    for (auto m : modes)
        mode += (m + "|");

    if (start == NOPOS)
        start = 0;

    if (get_config && set_config)
        return usage("Please chose only one of --set_config and --get_config. Quitting...");

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
                return usage("--depth parameter must be less than 4. Quitting...");
            replaceAll(mode, "|caches", "");
            ASSERT(endsWith(mode, '|'));
            bool hasAll = false;
            for (auto t : types) {
                hasAll |= (t == "all");
                if (t != "all")
                    mode += (t + "|");
            }
            mode += (hasAll ? "blocks|transactions|traces|slurps|prices|" : "");
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
        loadHashes(indexHashes, "finalized");
        loadHashes(bloomHashes, "blooms");
    }
    HIDE_FIELD(CChainCache, "max_depth");

    EXIT_NOMSG(true);
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOn(OPT_PREFUND | OPT_OUTPUT);
    // Since we need prefunds, let's load the names library here
    CAccountName unused;
    getNamedAccount(unused, "0x0");

    // BEG_CODE_INIT
    details = false;
    depth = NOPOS;
    start = NOPOS;
    end = NOPOS;
    // END_CODE_INIT

    isConfig = false;
    mode = "";

    char hostname[HOST_NAME_MAX];
    gethostname(hostname, HOST_NAME_MAX);
    char username[LOGIN_NAME_MAX];
    getlogin_r(username, LOGIN_NAME_MAX);
    if (!getEnvStr("DOCKER_MODE").empty()) {
        memset(username, 0, LOGIN_NAME_MAX);
        strncpy(username, "nobody", 7);
    }

    if (isTestMode()) {
        status.host = "--hostname-- (--username--)";
        status.rpc_provider = status.api_provider = status.balance_provider = "--providers--";
        status.cache_path = status.index_path = "--paths--";
    } else {
        status.host = string_q(hostname) + " (" + username + ")";
        status.rpc_provider = getGlobalConfig()->getConfigStr("settings", "rpcProvider", "http://localhost:8545");
        status.balance_provider = getGlobalConfig()->getConfigStr("settings", "balanceProvider", status.rpc_provider);
        status.api_provider = getGlobalConfig()->getConfigStr("settings", "apiProvider", "http://localhost:8080");
        status.cache_path = getGlobalConfig()->getConfigStr("settings", "cachePath", getCachePath(""));
        status.index_path = getGlobalConfig()->getConfigStr("settings", "indexPath", getIndexPath(""));
    }
    if (!isNodeRunning()) {
        status.client_version = "Not running";
    } else {
        status.client_version = (isTestMode() ? "Parity version" : getVersionFromClient());
    }
    status.trueblocks_version = getVersionStr();
    status.is_scraping = isTestMode() ? false : contains(listRunning("chifra scrape"), "chifra scrape");
    status.is_testing = isTestMode();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    setSorts(GETRUNTIME_CLASS(CBlock), GETRUNTIME_CLASS(CTransaction), GETRUNTIME_CLASS(CReceipt));
    Init();

    CStatus::registerClass();
    CCache::registerClass();
    CCollectionCache::registerClass();
    CCollectionCacheItem::registerClass();
    CIndexCache::registerClass();
    CIndexCacheItem::registerClass();
    CAbiCache::registerClass();
    CChainCache::registerClass();
    CMonitorCache::registerClass();
    CMonitorCacheItem::registerClass();
    CSlurpCache::registerClass();
    CPriceCache::registerClass();
    CPriceCacheItem::registerClass();
    CAbiCacheItem::registerClass();
    CConfiguration::registerClass();
    CConfigFile::registerClass();
    CConfigSection::registerClass();
    CConfigItem::registerClass();

    UNHIDE_FIELD(CAccountName, "nAppearances");
    UNHIDE_FIELD(CAccountName, "sizeInBytes");
    UNHIDE_FIELD(CAccountName, "firstAppearance");
    UNHIDE_FIELD(CAccountName, "latestAppearance");

    minArgs = 0;

    // BEG_CODE_NOTES
    // clang-format off
    // clang-format on
    // END_CODE_NOTES

    // BEG_ERROR_MSG
    // END_ERROR_MSG
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
