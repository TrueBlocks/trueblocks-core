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
#include "options_base.h"
#include "configenv.h"

namespace qblocks {

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

// TODO: BOGUS - multi-chain
//---------------------------------------------------------------------------------------------------
static CConfigEnv g_configEnv;
const CConfigEnv* getConfigEnv(void) {
    if (g_configEnv.configPath.empty()) {
        CStringArray fields = {
            "chain", "configPath", "chainConfigPath", "cachePath", "indexPath", "defChain", "rpcProvider",
        };
        string_q envStr = getEnvStr("TB_CONFIG_ENV");
        if (!g_configEnv.parseCSV(fields, envStr)) {
            LOG_ERR("Could not parse configEnv string: ", envStr);
        }
    }
    return &g_configEnv;
}

//---------------------------------------------------------------------------------------------------
string_q getPathToRootConfig(const string_q& _part) {
    string_q ret = getConfigEnv()->configPath;
    TEST_PATH(ret, _part, "Configuration");
    return ret + _part;
}

//--------------------------------------------------------------------------------------
string_q getPathToChainConfig(const string_q& _part) {
    string_q ret = getConfigEnv()->chainConfigPath;
    TEST_PATH(ret, _part, "Chain Configuration");
    return ret + _part;
}

//-------------------------------------------------------------------------
string_q getPathToCache(const string_q& _part) {
    string_q ret = getConfigEnv()->cachePath;
    TEST_PATH(ret, _part, "Cache");
    return ret + _part;
}

//-------------------------------------------------------------------------
string_q getPathToIndex(const string_q& _part) {
    string_q ret = getConfigEnv()->indexPath;
    TEST_PATH(ret, _part, "Index");
    return ret + _part;
}

//---------------------------------------------------------------------------------------------------
string_q getDefaultChain(void) {
    string_q ret = getConfigEnv()->defChain;
    ASSERT(!ret.empty());
    return ret;
}

//---------------------------------------------------------------------------------------------------
string_q getChain(void) {
    string_q ret = getConfigEnv()->chain;
    ASSERT(!ret.empty());
    return ret;
}

//---------------------------------------------------------------------------------------------------
string_q getRpcProvider(void) {
    string_q ret = getConfigEnv()->rpcProvider;
    ASSERT(!ret.empty());
    return ret;
}

//-------------------------------------------------------------------------
string_q getPathToCommands(const string_q& _part) {
    return getHomeFolder() + ".local/bin/chifra/" + _part;
}

// TODO: BOGUS - multi-chain
//-------------------------------------------------------------------------
// This routine is only used by tools that do not make their way through chifra.
// (makeClass and testRunner primarily). It mimics the way chifra works to build
// the configPaths. We ignore in this `chain`, defaulting to mainnet.
void loadEnvironmentPaths(void) {
#if defined(__linux) || defined(__linux__) || defined(linux) || defined(__unix) || defined(__unix__)
    string_q configPath = getHomeFolder() + ".local/share/trueblocks/";
#elif defined(__APPLE__) || defined(__MACH__)
    string_q configPath = getHomeFolder() + "Library/Application Support/TrueBlocks/";
#else
#error-- unknown operating system not supported
#endif
    // We need to set enough of the environment for us to get the RPC from the config file...
    ostringstream os1;
    os1 << "mainnet," << configPath << "," << (configPath + "config/mainnet/") << "," << (configPath + "cache/mainnet/")
        << "," << (configPath + "unchained/mainnet/") << ",mainnet,x";
    ::setenv("TB_CONFIG_ENV", os1.str().c_str(), true);
    string_q rpc = getGlobalConfig("")->getConfigStr("chains.mainnet", "rpcProvider", "http://localhost:8545");

    // Because `g_configEnv` is statis, we need to clear it...
    g_configEnv = CConfigEnv();  // reset so we get the rest

    // and reset it with the full env
    ostringstream os;
    os << "mainnet," << configPath << "," << (configPath + "config/mainnet/") << "," << (configPath + "cache/mainnet/")
       << "," << (configPath + "unchained/mainnet/") << ",mainnet," << rpc;
    ::setenv("TB_CONFIG_ENV", os.str().c_str(), true);
}

//-------------------------------------------------------------------------
string_q relativize(const string_q& path) {
    string_q ret = path;
    replace(ret, indexFolder, "$INDEX/");
    replace(ret, cacheFolder, "$CACHE/");
    replace(ret, chainConfigs, "$CHAIN/");
    replace(ret, rootConfigs, "$CONFIG/");
    replace(ret, getPathToCommands("test/"), "");
    replace(ret, getPathToCommands(""), "");
    replace(ret, getHomeFolder(), "$HOME/");
    return ret;
}

}  // namespace qblocks
