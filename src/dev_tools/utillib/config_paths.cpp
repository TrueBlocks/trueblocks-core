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
#include "filenames.h"
#include "exportcontext.h"
#include "logging.h"
#include "options_base.h"
#include "configenv.h"

namespace qblocks {

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
    return getConfigEnv()->configPath + _part;
}

//--------------------------------------------------------------------------------------
string_q getPathToChainConfig(const string_q& _part) {
    return getConfigEnv()->chainConfigPath + _part;
}

//-------------------------------------------------------------------------
string_q getPathToCache(const string_q& _part) {
    return getConfigEnv()->cachePath + _part;
}

//-------------------------------------------------------------------------
string_q getPathToIndex(const string_q& _part) {
    return getConfigEnv()->indexPath + _part;
}

//---------------------------------------------------------------------------------------------------
string_q getChain(void) {
    string_q ret = getConfigEnv()->chain;
    ASSERT(!ret.empty());
    return ret;
}

//-------------------------------------------------------------------------
string_q getPathToCommands(const string_q& _part) {
    return getHomeFolder() + ".local/bin/chifra/" + _part;
}

extern string_q getConfigPathLocal(void);
void loadEnvironmentPaths(void) {
    string_q chain = "mainnet";
    string_q configPath = getConfigPathLocal();
    ostringstream os1;
    os1 << "mainnet," << configPath << "," << (configPath + "config/mainnet/") << "," << (configPath + "cache/mainnet/")
        << "," << (configPath + "unchained/mainnet/") << ",mainnet,x";
    ::setenv("TB_CONFIG_ENV", os1.str().c_str(), true);
    string_q rpc = getGlobalConfig("")->getConfigStr("chains.mainnet", "rpcProvider", "http://localhost:8545");
    g_configEnv = CConfigEnv();  // reset so we get the rest
    string_q unchainedPath = configPath;
    string_q cachePath = configPath;
    ostringstream os;
    os << "mainnet," << configPath << "," << (configPath + "config/mainnet/") << "," << (cachePath + "cache/mainnet/")
       << "," << (unchainedPath + "unchained/mainnet/") << ",mainnet," << rpc;
    string_q env = os.str();
    if (!chain.empty()) {
        replaceAll(env, "mainnet", chain);
    }
    ::setenv("TB_CONFIG_ENV", env.c_str(), true);
}

//-------------------------------------------------------------------------
string_q relativize(const string_q& path) {
    string_q ret = path;
    replace(ret, getPathToIndex(""), "$INDEX/");
    replace(ret, cacheFolder, "$CACHE/");
    replace(ret, chainConfigs, "$CHAIN/");
    replace(ret, rootConfigs, "$CONFIG/");
    replace(ret, getPathToCommands("test/"), "");
    replace(ret, getPathToCommands(""), "");
    replace(ret, getHomeFolder(), "$HOME/");
    return ret;
}

//-------------------------------------------------------------------------
string_q getConfigPathLocal(void) {
#if defined(__linux) || defined(__linux__) || defined(linux) || defined(__unix) || defined(__unix__)
    string_q configPath = getHomeFolder() + ".local/share/trueblocks/";
#elif defined(__APPLE__) || defined(__MACH__)
    string_q configPath = getHomeFolder() + "Library/Application Support/TrueBlocks/";
#else
#error-- unknown operating system not supported
#endif
    return configPath;
}

}  // namespace qblocks
