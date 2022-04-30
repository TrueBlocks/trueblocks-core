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
bool migration_001(const string_q& path, void* data) {
    CMigrationChecker* checker = (CMigrationChecker*)data;
    if (endsWith(path, "/")) {
        return forEveryFileInFolder(path + "*", migration_001, data);

    } else {
        string_q tempFn = cacheFolder_tmp + "migrate";
        string_q pRelative = relativize(path);
        string_q tRelative = relativize(tempFn);

        if (endsWith(path, ".bin") && !contains(path, "/ts.bin")) {
            checker->nSeen++;

            if (checker->path == cacheFolder_names) {
                if (endsWith(path, ".bin")) {
                    checker->nMigrated++;
                    ::remove(path.c_str());
                }
                return true;
            }

            if (checker->path == cacheFolder_recons) {
                LOG_INFO("  Skipping '", pRelative, "'", "\r");
                checker->nMigrated++;
                return true;
            }

            CStringArray which;
            explode(which, checker->which, ',');
            for (auto w : which) {
                if (w == "0.17.0") {
                    if (contains(path, "monitors")) {
                        continue;
                    }

                    if (fileSize(path) == 0) {
                        checker->nSeen--;
                        ::remove(path.c_str());
                        continue;
                    }

                    lockSection();
                    CArchive readArchive(READING_ARCHIVE);
                    readArchive.Lock(path, modeReadOnly, LOCK_NOWAIT);
                    if (!readArchive.isOpen()) {
                        LOG_ERR("Could not open '", pRelative, "'");
                        unlockSection();
                        continue;
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
                            continue;
                        }

                        if (checker->path == cacheFolder_abis) {
                            CAbi item;
                            item.Migrate(readArchive, writeArchive);

                        } else if (checker->path == cacheFolder_slurps) {
                            CCachedAccount item;
                            item.Migrate(readArchive, writeArchive);

                        } else if (checker->path == cacheFolder_txs) {
                            CTransaction item;
                            item.Migrate(readArchive, writeArchive);

                        } else if (checker->path == cacheFolder_blocks) {
                            CBlock item;
                            item.Migrate(readArchive, writeArchive);

                        } else if (checker->path == cacheFolder_recons) {
                            CReconciliationArray items;
                            readArchive >> items;
                            writeArchive << items;

                        } else if (checker->path == cacheFolder_traces) {
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

                } else if (w == "0.30.0") {
                    if (endsWith(path, ".acct.bin")) {
                        ::remove(path.c_str());  // remove this so it doesn't cause a warning

                        lockSection();
                        string_q monFile = substitute(path, ".acct.bin", ".mon.bin");
                        string_q delFile = substitute(path, ".acct.bin", ".acct.bin.deleted");
                        string_q lastFile = substitute(path, ".acct.bin", ".last.txt");
                        if (!fileExists(monFile)) {
                            // Handle the situation differently if the new style monitor does not exists
                            CStringArray parts;
                            explode(parts, substitute(path, ".acct.bin", ""), '/');
                            address_t addr = parts[parts.size() - 1];
                            {  // keep the frame...
                                ostringstream cmd;
                                cmd << "chifra list --silent " << addr << " >/dev/null 2>&1 ";
                                if (system(cmd.str().c_str())) {
                                }  // Don't remove cruft. Silences compiler warnings
                            }
                            if (fileExists(delFile)) {
                                ostringstream cmd;
                                cmd << "chifra monitors --delete " << addr << " >/dev/null 2>&1 ";
                                if (system(cmd.str().c_str())) {
                                }  // Don't remove cruft. Silences compiler warnings
                            }
                        }
                        if (fileExists(delFile)) {
                            ::remove(delFile.c_str());
                        }
                        if (fileExists(lastFile)) {
                            ::remove(lastFile.c_str());
                        }
                        checker->nMigrated++;
                        LOG_INFO("  Migrated '", pRelative, "'");
                        unlockSection();
                    }
                }
            }

        } else {
            checker->nSkipped++;
        }
    }

    return !shouldQuit();
}

//--------------------------------------------------------------------------------
bool COptions::handle_migrate(const CStringArray& cachePaths) {
    LOG_INFO("Migrating caches at ", bBlue, cacheFolder, cOff);

    CMigrationChecker totals("", "");
    for (auto cache : cachePaths) {
        LOG_INFO(cGreen, "Checking '", relativize(cache), "'", string_q(50, ' '), cOff);
        CMigrationChecker checker(cache, "0.17.0,0.30.0");
        forEveryFileInFolder(cache, migration_001, &checker);  // will quit early if it finds a migrate
        LOG_INFO("  ", checker.Report() + string_q(30, ' '));
        totals += checker;
    }

    LOG_INFO(cGreen, "Finished...", cOff);
    LOG_INFO("  total", totals.Report());

    return false;
}
