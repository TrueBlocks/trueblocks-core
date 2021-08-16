/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//--------------------------------------------------------------------------------
bool migrateAbis(const string_q& path, void* data) {
    if (endsWith(path, "/")) {
        return forEveryFileInFolder(path + "*", migrateAbis, data);

    } else {
        // string_q relative = substitute(path, getCachePath(""), "$CACHE/");
        // if (endsWith(path, ".bin")) {
        //     lockSection(true);
        //     CArchive archiveIn(READING_ARCHIVE);
        //     if (!archiveIn(path, modeReadOnly, LOCK_NOWAIT)) {
        //         LOG_ERR("Could not open data file for migration: ", relative);
        //         return false;
        //     }
        //     CAbiArray abis;
        //     archiveIn >> abis;
        //     archiveIn.Release();
        //     lockSection(false);

        //     lockSection(true);
        //     CArchive archiveOut(WRITING_ARCHIVE);
        //     if (!archiveIn(getCachePath("tmp/migrate.bin"), modeWriteCreate, LOCK_NOWAIT)) {
        //         LOG_ERR("Could not open temp file for migration: ", getCachePath("tmp/migrate.bin"));
        //         return false;
        //     }
        //     lockSection(false);
        // }
    }
    return !shouldQuit();
}

//--------------------------------------------------------------------------------
bool COptions::handle_migrate(void) {
    // for (auto cache : cachePaths) {
    //     string_q path = getCachePath(cache);
    //     LOG_INFO(cGreen, "Checking '$CACHES/", cache, "'", string_q(50, ' '), cOff);
    //     CChecker checker(path);
    //     forEveryFileInFolder(path, needsMigrate, &checker);  // will quit early if it finds a migrate
    //     if (checker.needs) {
    //         LOG_WARN(" Cache '$CACHES/", cache, "' needs a migration.");
    //         needs = true;
    //     }
    // }
    return usage("Migrating");
}

/*
 monitors
 names
 prices
 slurps
 */
