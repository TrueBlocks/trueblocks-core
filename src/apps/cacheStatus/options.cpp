/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
// BEG_CODE_OPTIONS
    COption("mode_list", "", "list<enum[index|monitors|names|abis|blocks|transactions|traces|slurps|prices|some*|all]>", OPT_POSITIONAL, "one or more of [index|monitors|names|abis|blocks|transactions|traces|slurps|prices|some*|all]"),
    COption("details", "d", "", OPT_SWITCH, "include details about items found in monitors, slurps, abis, or price caches"),
    COption("list", "l", "", OPT_SWITCH, "display results in Linux ls -l format (assumes --detail)"),
    COption("start", "", "<blknum>", OPT_FLAG, "starting block for data retreival"),
    COption("fmt", "x", "enum[none|json*|txt|csv|api]", OPT_HIDDEN | OPT_FLAG, "export format (one of [none|json*|txt|csv|api])"),
    COption("config-get", "g", "", OPT_HIDDEN | OPT_SWITCH, "returns JSON data of the editable configuration file items"),
    COption("config-set", "s", "", OPT_HIDDEN | OPT_SWITCH, "accepts JSON in an env variable and writes it to configuration files"),
    COption("", "", "", OPT_DESCRIPTION, "Report on status of one or more TrueBlocks caches."),
// END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    ENTER("parseArguments");
    if (!standardOptions(command))
        EXIT_NOMSG(false);

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (arg == "-d" || arg == "--details") {
            details = true;

        } else if (arg == "-l" || arg == "--list") {
            isListing = true;

        } else if (startsWith(arg, "--start:")) {
            arg = substitute(arg, "--start:", "");
            if (!isNumeral(arg))
                return usage("'" + arg + "' is not a number for --start parameter. Quitting...");
            start = str_2_Uint(arg);
            CBlock block;
            getBlock_light(block, "latest");
            if (start > block.blockNumber)
                return usage("Start block (" + uint_2_Str(start) + ") is greater than the latest block. Quitting...");

        } else if (arg == "--config-get") {
            isConfig = true;

        } else if (startsWith(arg, "--config-set")) {
            mode = "set";
            isConfig = true;

        } else if (startsWith(arg, '-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {
            string_q permitted = params[0].description;
            replaceAny(permitted, "[]*", "|");
            if (!contains(permitted, "|" + arg + "|"))
                return usage("Provided value for 'mode' (" + arg + ") not " + substitute(params[0].description, "enum", "") + ". Quitting.");
            mode += (arg + "|");
        }
    }

    if (!isConfig) {
        if (mode.empty() || contains(mode, "some"))
            mode = "index|monitors|names|slurps|prices";

        if (contains(mode, "all"))
            mode = "index|monitors|names|abis|blocks|transactions|traces|slurps|prices";

        mode = "|" + trim(mode, '|') + "|";
    }

    if (!details) {
        HIDE_FIELD(CMonitorCache, "items");
        HIDE_FIELD(CSlurpCache, "items");
        HIDE_FIELD(CPriceCache, "items");
        HIDE_FIELD(CAbiCache, "items");
    }

    EXIT_NOMSG(true);
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOn(OPT_PREFUND | OPT_OUTPUT);

    isListing = false;
    isConfig = false;
    details = false;
    mode = "";
    start = 0;

    char hostname[HOST_NAME_MAX];  gethostname(hostname, HOST_NAME_MAX);
    char username[LOGIN_NAME_MAX]; getlogin_r(username, LOGIN_NAME_MAX);
    if (!getEnvStr("DOCKER_MODE").empty()) {
        memset(username, 0, LOGIN_NAME_MAX);
        strncpy(username, "nobody", 6);
    }

    if (isTestMode()) {
        status.host = "--hostname-- (--username--)";
        status.rpc_provider = status.api_provider = status.balance_provider = "--providers--";
    } else {
        status.host = string_q(hostname) + " (" + username + ")";
        status.rpc_provider = getGlobalConfig()->getConfigStr("settings", "rpcProvider", "http://localhost:8545");
        status.api_provider = getGlobalConfig()->getConfigStr("settings", "apiProvider", "http://localhost:8080");
        status.balance_provider = getGlobalConfig()->getConfigStr("settings", "balanceProvider", status.api_provider);
    }
    if (!isNodeRunning()) {
        status.client_version = "Not running";
    } else {
        status.client_version = (isTestMode() ? "Parity version" : getVersionFromClient());
    }
    status.trueblocks_version = getVersionStr();
    status.is_scraping = contains(listRunning("chifra scrape"), "chifra scrape");
    if (isTestMode())
        status.is_scraping = false;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();

    CStatus::registerClass();
    CCache::registerClass();
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
    CConfigGroup::registerClass();
    CConfigItem::registerClass();

    HIDE_FIELD(CAccountWatch,  "statement");
    HIDE_FIELD(CAccountWatch,  "stateHistory");
    HIDE_FIELD(CAccountWatch,  "curBalance");
    HIDE_FIELD(CAccountWatch,  "abi_spec");

    HIDE_FIELD(CAccountName,   "is_contract");
    HIDE_FIELD(CAccountName,   "is_private");
    HIDE_FIELD(CAccountName,   "is_shared");
    UNHIDE_FIELD(CAccountName, "nRecords");
    UNHIDE_FIELD(CAccountName, "nAppearances");
    UNHIDE_FIELD(CAccountName, "sizeInBytes");
    UNHIDE_FIELD(CAccountName, "firstAppearance");
    UNHIDE_FIELD(CAccountName, "latestAppearance");

    minArgs = 0;
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
