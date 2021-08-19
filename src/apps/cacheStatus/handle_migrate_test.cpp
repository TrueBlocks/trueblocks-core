/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//--------------------------------------------------------------------------------
bool needsMigrate(const string_q& path, void* data) {
    CMigrationChecker* checker = (CMigrationChecker*)data;
    if (endsWith(path, "/")) {
        return forEveryFileInFolder(path + "*", needsMigrate, data);

    } else {
        string_q relative = substitute(path, getCachePath(""), "$CACHE/");
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
        string_q path = getCachePath(cache);
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
