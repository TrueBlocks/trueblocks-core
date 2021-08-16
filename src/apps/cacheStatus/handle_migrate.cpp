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

            if (checker->type == "traces") {
                CArchive readArchive(READING_ARCHIVE);
                readArchive.Lock(path, modeReadOnly, LOCK_NOWAIT);
                if (!readArchive.isOpen()) {
                    LOG_ERR("Could not open '", pRelative, "'");
                    return false;
                }
                CTraceArray traces;
                readArchive >> traces;
                // The traces cache does not store a version number at the head of the file, so we
                // have to look at the first actual trace in the array for version information
                if (traces.size() > 0 && traces[0].m_schema < getVersionNum()) {
                    CArchive writeArchive(WRITING_ARCHIVE);
                    writeArchive.Lock(tempFn, modeWriteCreate, LOCK_NOWAIT);
                    if (!writeArchive.isOpen()) {
                        readArchive.Release();
                        LOG_ERR("Could not open '", tRelative, "'");
                        return false;
                    }
                    writeArchive << traces;
                    writeArchive.Release();
                    readArchive.Release();
                    moveFile(path, path + ".bak");
                    moveFile(tempFn, path);
                    checker->nMigrated++;
                    LOG_INFO("  Migrated '", pRelative, "'");
                }
                readArchive.Release();

            } else {
                if (checker->type == "abis") {
                    CArchive readArchive(READING_ARCHIVE);
                    readArchive.Lock(path, modeReadOnly, LOCK_NOWAIT);
                    if (!readArchive.isOpen()) {
                        LOG_ERR("Could not open '", pRelative, "'");
                        return false;
                    }
                    CArchive writeArchive(WRITING_ARCHIVE);
                    writeArchive.Lock(tempFn, modeWriteCreate, LOCK_NOWAIT);
                    if (!writeArchive.isOpen()) {
                        readArchive.Release();
                        LOG_ERR("Could not open '", tRelative, "'");
                        return false;
                    }
                    CAbi item;
                    item.Migrate(readArchive, writeArchive);
                    readArchive.Release();
                    writeArchive.Release();

                    moveFile(path, path + ".bak");
                    moveFile(tempFn, path);
                    checker->nMigrated++;
                    LOG_INFO("  Migrated '", pRelative, "'");
                } else if (checker->type == "slurps") {
                    CArchive readArchive(READING_ARCHIVE);
                    readArchive.Lock(path, modeReadOnly, LOCK_NOWAIT);
                    if (!readArchive.isOpen()) {
                        LOG_ERR("Could not open '", pRelative, "'");
                        return false;
                    }
                    CArchive writeArchive(WRITING_ARCHIVE);
                    writeArchive.Lock(tempFn, modeWriteCreate, LOCK_NOWAIT);
                    if (!writeArchive.isOpen()) {
                        readArchive.Release();
                        LOG_ERR("Could not open '", tRelative, "'");
                        return false;
                    }
                    CCachedAccount item;
                    item.Migrate(readArchive, writeArchive);
                    readArchive.Release();
                    writeArchive.Release();

                    moveFile(path, path + ".bak");
                    moveFile(tempFn, path);
                    checker->nMigrated++;
                    LOG_INFO("  Migrated '", pRelative, "'");
                } else if (checker->type == "txs") {
                    CArchive readArchive(READING_ARCHIVE);
                    readArchive.Lock(path, modeReadOnly, LOCK_NOWAIT);
                    if (!readArchive.isOpen()) {
                        LOG_ERR("Could not open '", pRelative, "'");
                        return false;
                    }
                    CArchive writeArchive(WRITING_ARCHIVE);
                    writeArchive.Lock(tempFn, modeWriteCreate, LOCK_NOWAIT);
                    if (!writeArchive.isOpen()) {
                        readArchive.Release();
                        LOG_ERR("Could not open '", tRelative, "'");
                        return false;
                    }
                    CBlock item;
                    item.Migrate(readArchive, writeArchive);
                    readArchive.Release();
                    writeArchive.Release();

                    moveFile(path, path + ".bak");
                    moveFile(tempFn, path);
                    checker->nMigrated++;
                    LOG_INFO("  Migrated '", pRelative, "'");
                }
            }

        } else {
            if (!endsWith(path, ".bak")) {
                checker->nSkipped++;
                LOG_INFO("  Skipping ", cYellow, pRelative, cOff);
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

    LOG_INFO(totals.Report());
    return false;
}
