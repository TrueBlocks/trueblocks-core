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
string_q getPathToConfig(const string_q& part) {
    static string_q g_configPath;
    if (!g_configPath.empty())
        return g_configPath + part;

#if defined(__linux) || defined(__linux__) || defined(linux) || defined(__unix) || defined(__unix__)
    g_configPath = getHomeFolder() + ".local/share/trueblocks/";
#elif defined(__APPLE__) || defined(__MACH__)
    g_configPath = getHomeFolder() + "Library/Application Support/TrueBlocks/";
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(_WIN64)
#error-- This source code does not compile on Windows
#else
#error-- unknown operating system not supported
#endif

    if (!endsWith(g_configPath, "/"))
        g_configPath += "/";
    replaceAll(g_configPath, "//", "/");
    return g_configPath + part;
}

//-------------------------------------------------------------------------
// TODO(tjayrush): global data
string_q getPathToCache(const string_q& _part) {
    static string_q g_cachePath;
    if (!g_cachePath.empty())
        return g_cachePath + _part;

    CToml toml(getPathToConfig("trueBlocks.toml"));
    string_q path = toml.getConfigStr("settings", "cachePath", "<not_set>");
    if (path == "<not_set>") {
        path = getPathToConfig("cache/");
        toml.setConfigStr("settings", "cachePath", path);
        toml.writeFile();
    }

    CFilename folder(path);
    if (!folderExists(folder.getFullPath()))
        establishFolder(folder.getFullPath());
    g_cachePath = folder.getFullPath();

    if (!folderExists(g_cachePath)) {
        cerr << "You've customized the cache path (" << g_cachePath << ")" << endl;
        cerr << "but it does not exist. Please create it first." << endl;
        quickQuitHandler(1);
    }

    if (!endsWith(g_cachePath, "/"))
        g_cachePath += "/";
    replaceAll(g_cachePath, "//", "/");
    return g_cachePath + _part;
}

extern void guardLiveTest(const string_q& path);
//-------------------------------------------------------------------------
string_q getPathToIndex(const string_q& _part) {
    static string_q g_indexPath;
    if (!g_indexPath.empty())
        return g_indexPath + _part;

    CToml toml(getPathToConfig("trueBlocks.toml"));
    string_q path = toml.getConfigStr("settings", "indexPath", "<not_set>");
    if (path == "<not_set>") {
        path = getPathToConfig("unchained/");
        toml.setConfigStr("settings", "indexPath", path);
        toml.writeFile();
    }

    CFilename folder(path);
    if (!folderExists(folder.getFullPath()))
        establishFolder(folder.getFullPath());
    g_indexPath = folder.getFullPath();

    if (!folderExists(g_indexPath)) {
        cerr << "You've customized the index path (" << g_indexPath << ")" << endl;
        cerr << "but it does not exist. Please create it first." << endl;
        quickQuitHandler(1);
    }

    if (!endsWith(g_indexPath, "/"))
        g_indexPath += "/";
    replaceAll(g_indexPath, "//", "/");
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
