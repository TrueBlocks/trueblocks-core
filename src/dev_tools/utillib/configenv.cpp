#include "configenv.h"
#include "filenames.h"

namespace qblocks {

ostream& operator<<(ostream& os, const CConfigEnv& it) {
    return os;
}

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
