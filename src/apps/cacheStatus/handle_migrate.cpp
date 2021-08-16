/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"
#include "cachedaccount.h"

//--------------------------------------------------------------------------------
bool migrateOne(const string_q& path, void* data) {
    CMigrationChecker* checker = (CMigrationChecker*)data;
    if (endsWith(path, "/")) {
        return forEveryFileInFolder(path + "*", migrateOne, data);

    } else {
        string_q tempFn = getCachePath("tmp/migrate");
        string_q pRelative = substitute(path, getCachePath(""), "$CACHE/");
        string_q tRelative = substitute(tempFn, getCachePath(""), "$CACHE/");

        if (endsWith(path, ".bin") && !contains(path, "/ts.bin")) {
            checker->nSeen++;
            if (fileSize(path) == 0) {
                ::remove(path.c_str());
                return true;
            }

            CArchive readArchive(READING_ARCHIVE);
            readArchive.Lock(path, modeReadOnly, LOCK_NOWAIT);
            if (!readArchive.isOpen()) {
                LOG_ERR("Could not open '", pRelative, "'");
                return false;
            }

            if (readArchive.needsUpgrade(contains(path, "/traces/") || contains(path, "/recons/"))) {
                CArchive writeArchive(WRITING_ARCHIVE);
                writeArchive.Lock(tempFn, modeWriteCreate, LOCK_NOWAIT);
                if (!writeArchive.isOpen()) {
                    readArchive.Release();
                    LOG_ERR("Could not open '", tRelative, "'");
                    return false;
                }

                if (checker->type == "abis") {
                    CAbi item;
                    item.Migrate(readArchive, writeArchive);
                    writeArchive.Release();

                } else if (checker->type == "slurps") {
                    CCachedAccount item;
                    item.Migrate(readArchive, writeArchive);
                    writeArchive.Release();

                } else if (checker->type == "txs") {
                    CTransaction item;
                    item.Migrate(readArchive, writeArchive);
                    writeArchive.Release();

                } else if (checker->type == "blocks") {
                    CBlock item;
                    item.Migrate(readArchive, writeArchive);
                    writeArchive.Release();

                } else if (checker->type == "recons") {
                    CReconciliationArray items;
                    readArchive >> items;
                    writeArchive << items;

                } else if (checker->type == "traces") {
                    CTraceArray items;
                    readArchive >> items;
                    writeArchive << items;
                }

                readArchive.Release();
                // moveFile(path, path + ".bak");
                moveFile(tempFn, path);
                checker->nMigrated++;
                LOG_INFO("  Migrated '", pRelative, "'");
            } else {
                LOG_INFO("  '", pRelative, "' does not need an upgrade", "\r");
                cerr.flush();
            }

        } else {
            if (!endsWith(path, ".bak")) {
                checker->nSkipped++;
                // LOG_INFO("  Skipping ", cYellow, pRelative, cOff);
            }
        }
    }

    return !shouldQuit();
}

//--------------------------------------------------------------------------------
bool COptions::handle_migrate(void) {
    CMigrationChecker totals("", "");
    for (auto cache : cachePaths) {
        string_q path = getCachePath(cache);
        LOG_INFO(cGreen, "Checking '$CACHES/", cache, "'", string_q(50, ' '), cOff);
        CMigrationChecker checker(path, cache);
        forEveryFileInFolder(path, migrateOne, &checker);  // will quit early if it finds a migrate
        LOG_INFO("  ", checker.Report() + string_q(30, ' '));
        totals += checker;
    }

    LOG_INFO(cGreen, "Finished...", cOff);
    LOG_INFO("  total", totals.Report());

    return false;
}
