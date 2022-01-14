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
#include "cachedaccount.h"

//--------------------------------------------------------------------------------
bool migrateOne(const string_q& path, void* data) {
    CMigrationChecker* checker = (CMigrationChecker*)data;
    if (endsWith(path, "/")) {
        return forEveryFileInFolder(path + "*", migrateOne, data);

    } else {
        string_q tempFn = cacheFolder_tmp + "migrate";
        string_q pRelative = substitute(path, cacheFolder, "$CACHE/");
        string_q tRelative = substitute(tempFn, cacheFolder, "$CACHE/");

        if (endsWith(path, ".bin") && !contains(path, "/ts.bin")) {
            checker->nSeen++;
            if (fileSize(path) == 0) {
                ::remove(path.c_str());
                return true;
            }

            if (checker->type == "names") {
                if (endsWith(path, ".bin")) {
                    checker->nMigrated++;
                    ::remove(path.c_str());
                }
                return true;
            }

            if (checker->type == "recons") {
                LOG_INFO("  Skipping '", pRelative, "'", "\r");
                checker->nMigrated++;
                return true;
            }

            lockSection();
            CArchive readArchive(READING_ARCHIVE);
            readArchive.Lock(path, modeReadOnly, LOCK_NOWAIT);
            if (!readArchive.isOpen()) {
                LOG_ERR("Could not open '", pRelative, "'");
                unlockSection();
                return false;
            }

            bool isTrace = contains(path, "/traces/");
            bool isRecon = contains(path, "/recons/");
            bool isNames = contains(path, "/names/");
            if (readArchive.needsUpgrade(isTrace || isRecon || isNames)) {
                CArchive writeArchive(WRITING_ARCHIVE);
                writeArchive.Lock(tempFn, modeWriteCreate, LOCK_NOWAIT);
                if (!writeArchive.isOpen()) {
                    readArchive.Release();
                    LOG_ERR("Could not open '", tRelative, "'");
                    unlockSection();
                    return false;
                }

                if (checker->type == "abis") {
                    CAbi item;
                    item.Migrate(readArchive, writeArchive);

                } else if (checker->type == "slurps") {
                    CCachedAccount item;
                    item.Migrate(readArchive, writeArchive);

                } else if (checker->type == "txs") {
                    CTransaction item;
                    item.Migrate(readArchive, writeArchive);

                } else if (checker->type == "blocks") {
                    CBlock item;
                    item.Migrate(readArchive, writeArchive);

                } else if (checker->type == "recons") {
                    CReconciliationArray items;
                    readArchive >> items;
                    writeArchive << items;

                } else if (checker->type == "traces") {
                    CTraceArray items;
                    readArchive >> items;
                    writeArchive << items;
                }

                writeArchive.Release();
                readArchive.Release();
                moveFile(tempFn, path);
                checker->nMigrated++;
                LOG_INFO("  Migrated '", pRelative, "'");
                unlockSection();
            } else {
                LOG_INFO("  '", pRelative, "' does not need an upgrade", "\r");
                cerr.flush();
            }

        } else {
            checker->nSkipped++;
        }
    }

    return !shouldQuit();
}

//--------------------------------------------------------------------------------
bool COptions::handle_migrate(void) {
    CMigrationChecker totals("", "");
    for (auto cache : cachePaths) {
        string_q path = getPathToCache(cache);
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
