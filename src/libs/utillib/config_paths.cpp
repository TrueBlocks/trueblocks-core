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

namespace qblocks {

// TODO: Search for PathAccessor

//---------------------------------------------------------------------------------------------------
// TODO(tjayrush): global data
string_q getPathToConfig(const string_q& _part) {
    static string_q g_configPath;
    if (!g_configPath.empty())
        return g_configPath + _part;

    // if (!isTestMode())
    //     cerr << bGreen << "TB_CONFIG_PATH: " << getEnvStr("TB_CONFIG_PATH") << cOff << endl;

    g_configPath = substitute(getEnvStr("TB_CONFIG_PATH"), "mainnet/", "");
    return g_configPath + _part;
}

//-------------------------------------------------------------------------
// TODO(tjayrush): global data
string_q getPathToCache(const string_q& _part) {
    static string_q g_cachePath;
    if (!g_cachePath.empty())
        return g_cachePath + _part;

    // if (!isTestMode())
    //     cerr << bGreen << "TB_CACHE_PATH: " << getEnvStr("TB_CACHE_PATH") << cOff << endl;

    g_cachePath = substitute(getEnvStr("TB_CACHE_PATH"), "mainnet/", "");
    return g_cachePath + _part;
}

extern void guardLiveTest(const string_q& path);
//-------------------------------------------------------------------------
string_q getPathToIndex(const string_q& _part) {
    static string_q g_indexPath;
    if (!g_indexPath.empty())
        return g_indexPath + _part;

    // if (!isTestMode())
    //     cerr << bGreen << "TB_INDEX_PATH: " << getEnvStr("TB_INDEX_PATH") << cOff << endl;

    g_indexPath = substitute(getEnvStr("TB_INDEX_PATH"), "mainnet/", "");
    return g_indexPath + _part;
}

//-------------------------------------------------------------------------
string_q getPathToCommands(const string_q& _part) {
    return getHomeFolder() + ".local/bin/chifra/" + _part;
}

//-------------------------------------------------------------------------
void loadEnvironmentPaths(void) {
    // This is only called by makeClass and testRunner (the only two tools that do not run through chifra). It
    // mimics the way chifra works to build the configPath so these two tools will run
#if defined(__linux) || defined(__linux__) || defined(linux) || defined(__unix) || defined(__unix__)
    string_q configPath = getHomeFolder() + ".local/share/trueblocks/";
#elif defined(__APPLE__) || defined(__MACH__)
    string_q configPath = getHomeFolder() + "Library/Application Support/TrueBlocks/";
#else
#error-- unknown operating system not supported
#endif
    ::setenv("TB_CONFIG_PATH", configPath.c_str(), true);
    ::setenv("TB_CACHE_PATH", (configPath + "cache/").c_str(), true);
    ::setenv("TB_INDEX_PATH", (configPath + "unchained/").c_str(), true);
}

}  // namespace qblocks
