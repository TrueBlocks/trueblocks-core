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
    COption("modes", "", "list<enum[index|monitors|names|abis|slurps|blocks|txs|traces|some*|all]>", OPT_POSITIONAL, "the (optional) name of the binary cache to report on, terse otherwise"),  // NOLINT
    COption("", "", "", OPT_DESCRIPTION, "Report on the state of the internal binary caches."),
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    if (!standardOptions(command))
        return false;

    // BEG_CODE_LOCAL_INIT
    CStringArray modes;
    // END_CODE_LOCAL_INIT

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
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
        cacheFolder_recons,
        cacheFolder_slurps,
        /* cacheFolder_tmp, */
        cacheFolder_traces,
        cacheFolder_txs,
    };

    if (!loadNames())
        return usage("Could not load names database.");

    establishIndexFolders();
    establishCacheFolders();

    for (auto m : modes) {
        mode += (m + "|");
    }
    origMode = mode;

    if (mode.empty() || contains(mode, "some")) {
        mode = "index|monitors|names|abis|slurps";

    } else if (contains(mode, "all")) {
        mode = "index|monitors|names|abis|slurps|blocks|txs|traces";
    }
    mode = "|" + trim(mode, '|') + "|";

    HIDE_FIELD(CAbiCache, "items");
    HIDE_FIELD(CChainCache, "items");
    HIDE_FIELD(CIndexCache, "items");
    HIDE_FIELD(CMonitorCache, "items");
    HIDE_FIELD(CNameCache, "items");
    HIDE_FIELD(CSlurpCache, "items");

    blknum_t first_block = 0;
    blknum_t last_block = NOPOS;
    if (isTestMode()) {
        first_block = 1000000;
        last_block = 2200000;
    }

    for (auto m : modes) {
        if (m == "all") {
            SHOW_FIELD(CAbiCache, "items");
            SHOW_FIELD(CChainCache, "items");
            SHOW_FIELD(CIndexCache, "items");
            SHOW_FIELD(CMonitorCache, "items");
            SHOW_FIELD(CNameCache, "items");
            SHOW_FIELD(CSlurpCache, "items");
            SHOW_FIELD(CMonitorCacheItem, "deleted");
            first_block = 0;
            last_block = NOPOS;
        } else if (m == "some") {
            SHOW_FIELD(CIndexCache, "items");
            SHOW_FIELD(CMonitorCache, "items");
            SHOW_FIELD(CNameCache, "items");
            SHOW_FIELD(CSlurpCache, "items");
            SHOW_FIELD(CMonitorCacheItem, "deleted");
            first_block = 0;
            last_block = NOPOS;
        } else if (m == "abis") {
            SHOW_FIELD(CAbiCache, "items");
        } else if (m == "caches") {
            SHOW_FIELD(CChainCache, "items");
        } else if (m == "index") {
            SHOW_FIELD(CIndexCache, "items");
            first_block = 0;
            last_block = NOPOS;
        } else if (m == "monitors") {
            SHOW_FIELD(CMonitorCache, "items");
            SHOW_FIELD(CMonitorCacheItem, "deleted");
        } else if (m == "names") {
            SHOW_FIELD(CNameCache, "items");
        } else if (m == "slurps") {
            SHOW_FIELD(CSlurpCache, "items");
        } else if (m == "blocks") {
            SHOW_FIELD(CSlurpCache, "items");
        } else if (m == "txs") {
            SHOW_FIELD(CSlurpCache, "items");
        } else if (m == "traces") {
            SHOW_FIELD(CSlurpCache, "items");
        }
    }
    scanRange = make_pair(first_block, last_block);

    if (isTestMode()) {
        HIDE_FIELD(CChain, "ipfsGateway");
    }
    HIDE_FIELD(CChainCache, "max_depth");

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    // BEG_CODE_GLOBALOPTS
    registerOptions(nParams, params, OPT_PREFUND);
    // END_CODE_GLOBALOPTS

    // BEG_CODE_INIT
    // END_CODE_INIT

    meta = getMetaData();

    scanRange = make_pair(0, NOPOS);
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
    if (getlogin_r(username, LOGIN_NAME_MAX) != 0)
        strncpy(username, "nobody", 7);

    if (isTestMode()) {
        status.host = "--hostname-- (--username--)";
        status.rpcProvider = "--providers--";
        status.configPath = status.cachePath = status.indexPath = "--paths--";
    } else {
        status.host = string_q(hostname) + " (" + username + ")";
        status.rpcProvider = getRpcProvider();
        status.configPath = rootConfigs;
        status.cachePath = cacheFolder;
        status.indexPath = indexFolder;
    }
    if (!isNodeRunning()) {
        status.clientVersion = "Not running";
        status.clientIds = "Not running";
    } else {
        status.clientVersion = (isTestMode() ? "Client version" : getVersionFromClient());
        status.clientIds = "chainId: " + uint_2_Str(meta.chainId) + " networkId: " + uint_2_Str(meta.networkId);
    }
    status.trueblocksVersion = getVersionStr();
    status.isScraping = false;  // isTestMode() ? false : (isRunning("chifra scrape") || isRunning("blockScrape"));
    status.isTesting = isTestMode();
    status.isApi = isApiMode();
    status.isArchive = isArchiveNode();
    status.isTracing = isTracingNode();
    status.hasEskey = getGlobalConfig("")->getConfigStr("keys.etherscan", "apiKey", "<not_set>") != "<not_set>";
    status.hasPinkey =
        getGlobalConfig("blockScrape")->getConfigStr("keys.pinata", "apiKey", "<not_set>") != "<not_set>";
    if (isTestMode())
        status.hasPinkey = false;
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
    CNameCache::registerClass();
    CSlurpCache::registerClass();
    CAbiCacheItem::registerClass();
    CChain::registerClass();
    CKey::registerClass();

    UNHIDE_FIELD(CCacheBase, "nApps");
    UNHIDE_FIELD(CCacheBase, "sizeInBytes");
    UNHIDE_FIELD(CCacheBase, "firstApp");
    UNHIDE_FIELD(CCacheBase, "latestApp");

    minArgs = 0;

    // BEG_CODE_NOTES
    // clang-format off
    notes.push_back("The `some` mode includes index, monitors, names, slurps, and abis.");
    // clang-format on
    // END_CODE_NOTES

    // BEG_ERROR_STRINGS
    // END_ERROR_STRINGS
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
