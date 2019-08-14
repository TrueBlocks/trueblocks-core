/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
// BEG_CODE_OPTIONS
    COption("mode_list", "", "list<enum[scraper|monitors|names|abis|blocks|txs|traces|slurps|prices|some*|all]>", OPT_REQUIRED | OPT_POSITIONAL, "one or more of [scraper|monitors|names|abis|blocks|txs|traces|slurps|prices|some*|all]"),
    COption("details", "d", "", OPT_SWITCH, "include details about items found in monitors, slurps, abis, or price caches"),
    COption("list", "l", "", OPT_SWITCH, "display results in Linux ls -l format (assumes --detail)"),
    COption("fmt", "x", "enum[none|json*|txt|csv|api]", OPT_HIDDEN | OPT_FLAG, "export format (one of [none|json*|txt|csv|api])"),
    COption("", "", "", 0, "Report on status of one or more TrueBlocks caches."),
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
            ls = true;

        } else if (startsWith(arg, '-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {
            string_q options = params[0].description;
            replaceAny(options, "[]*", "|");
            if (!contains(options, "|" + arg + "|"))
                return usage("Provided value for 'mode' (" + arg + ") not " + substitute(params[0].description, "enum", "") + ". Quitting.");
            mode += (arg + "|");
        }
    }

    if (mode.empty() || contains(mode, "some"))
        mode = "scraper|monitors|names|abis|slurps|prices";

    if (contains(mode, "all"))
        mode = "scraper|monitors|names|abis|blocks|txs|traces|slurps|prices";

    mode = "|" + trim(mode, '|') + "|";

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
    optionOff(OPT_VERBOSE);

    ls = false;
    details = false;
    mode = "";
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
    status.client_version = getVersionFromClient();
    status.trueblocks_version = getVersionStr();
    status.is_scraping = isRunning("chifra scrape", true);
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();

    CStatus::registerClass();
    CCache::registerClass();
    CAddressCache::registerClass();
    CAbiCache::registerClass();
    CChainCache::registerClass();
    CMonitorCache::registerClass();
    CMonitorCacheItem::registerClass();
    CSlurpCache::registerClass();
    CPriceCache::registerClass();
    CPriceCacheItem::registerClass();
    CAbiCacheItem::registerClass();

    HIDE_FIELD(CAccountWatch,  "statement");
    HIDE_FIELD(CAccountWatch,  "stateHistory");
    HIDE_FIELD(CAccountWatch,  "nodeBal");
    HIDE_FIELD(CAccountWatch,  "abi_spec");

    HIDE_FIELD(CAccountName,   "is_contract");
    HIDE_FIELD(CAccountName,   "is_custom");
    HIDE_FIELD(CAccountName,   "is_shared");
    UNHIDE_FIELD(CAccountName, "nRecords");
    UNHIDE_FIELD(CAccountName, "sizeInBytes");
    UNHIDE_FIELD(CAccountName, "firstAppearance");
    UNHIDE_FIELD(CAccountName, "latestAppearance");

    minArgs = 0;
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
