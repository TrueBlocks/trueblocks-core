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
            CArchive archive(READING_ARCHIVE);
            if (archive.Lock(path, modeReadOnly, LOCK_NOWAIT)) {
                checker->needs = archive.needsUpgrade(contains(path, "/traces/") || contains(path, "/recons/"));
                LOG_INFO("  Checking '", relative, "' isCurrent: ", cBlue, (checker->needs ? "false" : "true"), cOff,
                         (checker->needs ? "" : "\r"));
                if (checker->needs || !shouldQuit())
                    return false;  // quit after we find the first one that needs migrate
            }
            // } else {
            //     LOG_INFO("  Skipping ", cYellow, relative, cOff, "\r");
            //     cerr.flush();
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
            LOG_WARN("  Cache '$CACHES/", cache, "' needs a migration.");
        } else {
            LOG_WARN("  Cache '$CACHES/", cache, "' is up to date.", string_q(70, ' '));
        }
    }

    return false;
}
