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
#include "options.h"

//--------------------------------------------------------------------------------
bool needsMigrate(const string_q& path, void* data) {
    CMigrationChecker* checker = (CMigrationChecker*)data;
    if (endsWith(path, "/")) {
        return forEveryFileInFolder(path + "*", needsMigrate, data);

    } else {
        string_q relative = substitute(path, cacheFolder, "$CACHE/");
        if (endsWith(path, ".bin") && !contains(path, "/ts.bin")) {
            CArchive readArchive(READING_ARCHIVE);
            if (readArchive.Lock(path, modeReadOnly, LOCK_NOWAIT)) {
                bool isTrace = contains(path, "/traces/");
                bool isRecon = contains(path, "/recons/");
                bool isNames = contains(path, "/names/");
                checker->needs = readArchive.needsUpgrade(isTrace || isRecon || isNames);
                LOG_INFO("  Checking '", relative, "'", (checker->needs ? "" : "\r"));
                readArchive.Release();
                if (checker->needs)
                    return false;  // quit after we find the first one that needs migrate
            }
        }
    }
    return !shouldQuit();
}

//--------------------------------------------------------------------------------
bool COptions::handle_migrate_test(void) {
    for (auto cache : cachePaths) {
        string_q path = getPathToCache(cache);
        LOG_INFO(cGreen, "Checking '$CACHES/", cache, "'", string_q(50, ' '), cOff);
        CMigrationChecker checker(path, cache);
        forEveryFileInFolder(path, needsMigrate, &checker);  // will quit early if it finds a migrate
        if (checker.needs) {
            LOG_WARN(cYellow, "  Cache '$CACHES/", cache, "' needs a migration.", cOff);
        } else {
            LOG_WARN("  Cache '$CACHES/", cache, "' is up to date.", string_q(70, ' '));
        }
    }

    return false;
}
