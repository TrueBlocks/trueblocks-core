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
    COption("modes", "", "list<enum[index|monitors|names|abis|caches|some*|all]>", OPT_POSITIONAL, "the type of status info to retrieve"),  // NOLINT
    COption("details", "d", "", OPT_SWITCH, "include details about items found in monitors, slurps, abis, or price caches"),  // NOLINT
    COption("types", "t", "list<enum[blocks|txs|traces|slurps|all*]>", OPT_FLAG, "for caches mode only, which type(s) of cache to report"),  // NOLINT
    COption("depth", "p", "<uint64>", OPT_HIDDEN | OPT_FLAG, "for cache mode only, number of levels deep to report"),
    COption("terse", "e", "", OPT_HIDDEN | OPT_SWITCH, "show a terse summary report"),
    COption("migrate", "m", "enum[test|cache|index]", OPT_HIDDEN | OPT_FLAG, "either effectuate or test to see if a migration is necessary"),  // NOLINT
    COption("first_block", "F", "<blknum>", OPT_HIDDEN | OPT_FLAG, "first block to process (inclusive -- testing only)"),  // NOLINT
    COption("last_block", "L", "<blknum>", OPT_HIDDEN | OPT_FLAG, "last block to process (inclusive -- testing only)"),
    COption("", "", "", OPT_DESCRIPTION, "Report on the status of the TrueBlocks system."),
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
    CStringArray types;
    string_q migrate = "";
    blknum_t first_block = 0;
    blknum_t last_block = NOPOS;
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
        cacheFolder_recons,
        cacheFolder_slurps,
        /* cacheFolder_tmp, */
        cacheFolder_traces,
        cacheFolder_txs,
    };
    if (migrate == "test") {
        handle_migrate_test(cachePaths);
        return false;
    } else if (migrate == "cache") {
        handle_migrate(cachePaths);
        return false;
    } else if (!migrate.empty()) {
        // do nothing here for --migrate index. It's an error if
        // it got this far. It should have been handled in the go code
        return usage("Invalid migration: " + migrate);
    }

    if (!loadNames())
        return usage("Could not load names database.");

    establishIndexFolders();
    establishCacheFolders();

    for (auto m : modes)
        mode += (m + "|");
    origMode = mode;

    if (!isTestMode() && (first_block != 0 || (last_block != NOPOS && last_block != 0)))
        return usage("--first_block (" + uint_2_Str(first_block) + ") and --last_block (" + uint_2_Str(last_block) +
                     ") are only available during testing.");
    scanRange = make_pair(first_block, last_block);

    if (mode.empty() || contains(mode, "some"))
        mode = "index|monitors|names|slurps";
    if (contains(mode, "all")) {
        mode = "index|monitors|names|abis|caches";
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
        mode += (hasAll ? "blocks|txs|traces|slurps|" : "");
    }

    if (!details) {
        HIDE_FIELD(CMonitorCache, "items");
        HIDE_FIELD(CSlurpCache, "items");
        HIDE_FIELD(CAbiCache, "items");
        HIDE_FIELD(CChainCache, "items");
    }
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
    details = false;
    depth = NOPOS;
    terse = false;
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
    status.isScraping = isTestMode() ? false : (isRunning("chifra scrape") || isRunning("blockScrape"));
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
    CStatusTerse::registerClass();
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

//---------------------------------------------------------------------------
bool countFilesInCache(const string_q& path, void* data) {
    CChainCache* counter = reinterpret_cast<CChainCache*>(data);
    if (endsWith(path, '/')) {
        if (contains(path, "/0")) {
            uint64_t d = countOf(path, '/') - 1;
            uint64_t m = counter->max_depth;
            if (d == m) {
                if (isTestMode()) {
                    counter->items.push_back("Testing/00/00/00");
                    counter->items.push_back("Testing/00/01/00");
                    return false;
                } else {
                    counter->items.push_back(substitute(path, counter->path, ""));
                }
            }
            if (!isTestMode()) {
                counter->noteFolder(path);
            }
        }
        return forEveryFileInFolder(path + "*", countFilesInCache, data);

    } else {
        if (!isTestMode())
            counter->noteFile(path);
        counter->isValid = true;
        if (isTestMode()) {
            counter->items.push_back("Testing/00/00/00/file1.bin");
            counter->items.push_back("Testing/00/01/00/file2.bin");
            return false;

        } else if (counter->max_depth == countOf(path, '/')) {
            counter->items.push_back(substitute(path, counter->path, ""));
        }
    }
    return !shouldQuit();
}

//---------------------------------------------------------------------------
bool countFiles(const string_q& path, void* data) {
    CCache* counter = reinterpret_cast<CCache*>(data);
    if (endsWith(path, '/')) {
        if (!isTestMode() && !contains(path, "monitors/staging"))
            counter->noteFolder(path);
        return forEveryFileInFolder(path + "*", countFiles, data);

    } else if (endsWith(path, ".bin") || endsWith(path, ".json")) {
        if (!isTestMode())
            counter->noteFile(path);
        counter->isValid = true;
    }
    return !shouldQuit();
}
