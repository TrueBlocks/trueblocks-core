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
#include "basetypes.h"
#include "database.h"
#include "exportcontext.h"
#include "config_paths.h"
#include "colors.h"
#include "filenames.h"
#include "accountname.h"
#include "rpcresult.h"
#include "exportcontext.h"
#include "logging.h"

namespace qblocks {

// TODO: Search for PathAccessor

// TODO: This can be removed once multi-chain works
//---------------------------------------------------------------------------------------------------
#define TEST_PATH(path, part, type)                                                                                    \
    if (!isTestMode()) {                                                                                               \
        if (!folderExists((path))) {                                                                                   \
            LOG_ERR(string_q(type) + " folder must exist: ", (path));                                                  \
            quickQuitHandler(1);                                                                                       \
        }                                                                                                              \
        if (!(part).empty() && !fileExists((path) + (part))) {                                                         \
            LOG_ERR(string_q(type) + " part is missing: ", (path) + (part));                                           \
            quickQuitHandler(1);                                                                                       \
        }                                                                                                              \
        if (!endsWith((path), "/")) {                                                                                  \
            LOG_ERR(string_q(type) + " folder must end with '/': ", (path));                                           \
            quickQuitHandler(1);                                                                                       \
        }                                                                                                              \
    }

//---------------------------------------------------------------------------------------------------
string_q getPathToChainConfig_newOff(const string_q& _part) {
    return getPathToRootConfig(_part);
}

//--------------------------------------------------------------------------------------
static string_q g_chainConfigPath;
string_q getPathToChainConfig_new(const string_q& _part) {
    if (!g_chainConfigPath.empty())
        return g_chainConfigPath + _part;
    g_chainConfigPath = getEnvStr("TB_CHAIN_CONFIG_PATH");
    LOG4(bGreen, "c-CHAIN_CONFIG_PATH: ", isTestMode() ? relativize(g_chainConfigPath) : g_chainConfigPath, cOff);
    TEST_PATH(g_chainConfigPath, _part, "Chain Configuration");
    return g_chainConfigPath + _part;
}

//---------------------------------------------------------------------------------------------------
// TODO(tjayrush): Remove this comment once multi-path works - PathAccessor
static string_q g_configPath;
string_q getPathToRootConfig(const string_q& _part) {
    if (!g_configPath.empty())
        return g_configPath + _part;
    g_configPath = getEnvStr("TB_CONFIG_PATH");
    LOG4(bGreen, "c-CONFIG_PATH: ", isTestMode() ? relativize(g_configPath) : g_configPath, cOff);
    TEST_PATH(g_configPath, _part, "Configuration");
    return g_configPath + _part;
}

//-------------------------------------------------------------------------
// TODO(tjayrush): Remove this comment once multi-path works - PathAccessor
static string_q g_cachePath;
string_q getPathToCache(const string_q& _part) {
    if (!g_cachePath.empty())
        return g_cachePath + _part;
    g_cachePath = getEnvStr("TB_CACHE_PATH");
    LOG4(bGreen, "c-CACHE_PATH: ", isTestMode() ? relativize(g_cachePath) : g_cachePath, cOff);
    TEST_PATH(g_cachePath, _part, "Cache");
    return g_cachePath + _part;
}

//-------------------------------------------------------------------------
// TODO(tjayrush): Remove this comment once multi-path works - PathAccessor
static string_q g_indexPath;
string_q getPathToIndex(const string_q& _part) {
    if (!g_indexPath.empty())
        return g_indexPath + _part;
    g_indexPath = getEnvStr("TB_INDEX_PATH");
    LOG4(bGreen, "c-INDEX_PATH: ", isTestMode() ? relativize(g_indexPath) : g_indexPath, cOff);
    TEST_PATH(g_indexPath, _part, "Index");
    return g_indexPath + _part;
}

//-------------------------------------------------------------------------
string_q getPathToCommands(const string_q& _part) {
    return getHomeFolder() + ".local/bin/chifra/" + _part;
}

//-------------------------------------------------------------------------
// TODO(tjayrush): Remove this comment once multi-path works - PathAccessor
void loadEnvironmentPaths(void) {
    // This is only called by makeClass and testRunner (the only two tools that do not run through chifra). It
    // mimics the way chifra works to build the configPath so these two tools will run. It ignore the
    // `chain` parameter that is used to build these paths in `chifra` as these two tools don't need them.
#if defined(__linux) || defined(__linux__) || defined(linux) || defined(__unix) || defined(__unix__)
    string_q configPath = getHomeFolder() + ".local/share/trueblocks/";
#elif defined(__APPLE__) || defined(__MACH__)
    string_q configPath = getHomeFolder() + "Library/Application Support/TrueBlocks/";
#else
#error-- unknown operating system not supported
#endif
    ::setenv("TB_CONFIG_PATH", configPath.c_str(), true);
    ::setenv("TB_CHAIN_CONFIG_PATH", (configPath + "config/mainnet/").c_str(), true);
    ::setenv("TB_CACHE_PATH", (configPath + "cache/").c_str(), true);
    ::setenv("TB_INDEX_PATH", (configPath + "unchained/").c_str(), true);
}

//-------------------------------------------------------------------------
string_q relativize(const string_q& path) {
    string_q ret = path;
    replace(ret, indexFolder, "$INDEX/");
    replace(ret, cacheFolder, "$CACHE/");
    replace(ret, getPathToChainConfig_new(""), "$CHAIN/");
    replace(ret, getPathToRootConfig(""), "$CONFIG/");
    replace(ret, getPathToCommands("test/"), "");
    replace(ret, getPathToCommands(""), "");
    replace(ret, getHomeFolder(), "$HOME/");
    return ret;
}

}  // namespace qblocks
