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
extern bool isRunning(const string_q& prog);
static const COption params[] = {
    // BEG_CODE_OPTIONS
    // clang-format off
    COption("mode", "", "enum[index|monitors|names|abis|caches|some*|all]", OPT_POSITIONAL, "the (optional) name of the binary cache to report on, terse otherwise"),  // NOLINT
    COption("types", "t", "list<enum[blocks|txs|traces|slurps|all*]>", OPT_FLAG, "for caches mode only, which type(s) of cache to report"),  // NOLINT
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
    CStringArray types;
    // END_CODE_LOCAL_INIT

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (startsWith(arg, "-t:") || startsWith(arg, "--types:")) {
            string_q types_tmp;
            if (!confirmEnum("types", types_tmp, arg))
                return false;
            types.push_back(types_tmp);
        } else if (arg == "-t" || arg == "--types") {
            return flag_required("types");

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return invalid_option(arg);
            }

        } else {
            if (!mode.empty())
                return usage("Please specify only one mode.");
            if (!confirmEnum("mode", mode, arg))
                return false;

            // END_CODE_AUTO
        }
    }

    // cerr << string_q(120, '-') << endl;
    // cerr << mode << endl;
    // cerr << string_q(120, '-') << endl;

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

    origMode = mode;

    if (!verbose && (mode.empty() || contains(mode, "some"))) {
        mode = "index|monitors|names|slurps";

    } else if (contains(mode, "all")) {
        mode = "index|monitors|names|abis|caches";
        types.push_back("all");
    }

    mode = "|" + trim(mode, '|') + "|";

    if (contains(mode, "|caches")) {
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

    // if (!verbose && mode.empty()) {
    //     HIDE_FIELD(CSlurpCache, "items");
    //     HIDE_FIELD(CAbiCache, "items");
    //     HIDE_FIELD(CChainCache, "items");
    //     HIDE_FIELD(CAbiCache, "items");
    //     HIDE_FIELD(CAbiCacheItem, "items");
    //     HIDE_FIELD(CChainCache, "items");
    //     HIDE_FIELD(CIndexCache, "items");
    //     HIDE_FIELD(CMonitorCache, "items");
    //     HIDE_FIELD(CNameCache, "items");
    //     HIDE_FIELD(CSlurpCache, "items");
    // }

    blknum_t first_block = 0;
    blknum_t last_block = NOPOS;
    if (isTestMode()) {
        first_block = 1000000;
        last_block = 2200000;
    }

    if (mode == "monitors" || mode == "some" || mode == "all") {
        SHOW_FIELD(CMonitorCache, "items");
    }
    if (mode == "index" || mode == "some" || mode == "all") {
        SHOW_FIELD(CIndexCache, "items");
        first_block = 0;
        last_block = NOPOS;
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
    }
    return !shouldQuit();
}

//----------------------------------------------------------------------------
bool isRunning(const string_q& progName) {
    string_q cmd = "pgrep -lfa \"" + progName + "\"";
    string_q result = doCommand(cmd);
    CStringArray lines;
    explode(lines, result, '\n');
    result = "";
    for (auto line : lines) {
        if (!contains(line, "sh -c")) {
            // each line is of form: 'pid program options'. Skip over pid
            nextTokenClear(line, ' ');
            if (startsWith(line, progName)) {
                result += line + "\n";
            }
        }
    }
    if (isTestMode())
        LOG4("\n", cmd, "\n", result, " ", !result.empty());
    return contains(result, progName);
}
