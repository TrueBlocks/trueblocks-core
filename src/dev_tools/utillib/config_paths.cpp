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
// #include "logging.h"
#include "options_base.h"
#include "configenv.h"

namespace qblocks {

static CConfigEnv g_configEnv;
string_q getPathToCommands(const string_q& _part) {
    return getHomeFolder() + ".local/bin/chifra/" + _part;
}

const CConfigEnv* getConfigEnv(void) {
    return &g_configEnv;
}

void loadEnvironmentPaths(void) {
    g_configEnv.chain = "mainnet";
#if defined(__linux) || defined(__linux__) || defined(linux) || defined(__unix) || defined(__unix__)
    g_configEnv.configPath = getHomeFolder() + ".local/share/trueblocks/";
#elif defined(__APPLE__) || defined(__MACH__)
    g_configEnv.configPath = getHomeFolder() + "Library/Application Support/TrueBlocks/";
#endif
    g_configEnv.chainConfigPath = g_configEnv.configPath + "config/mainnet/";
    g_configEnv.cachePath = g_configEnv.configPath + "cache/mainnet/";
}

}  // namespace qblocks
