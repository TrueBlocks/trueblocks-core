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
bool needsMigration(const string_q& path, void* data) {
    CMigrationChecker* checker = (CMigrationChecker*)data;
    if (endsWith(path, "/")) {
        return forEveryFileInFolder(path + "*", needsMigration, data);

    } else {
        if (endsWith(path, ".bin") && !contains(path, "/ts.bin")) {
            CStringArray which;
            explode(which, checker->which, ',');
            for (auto w : which) {
                if (w == "0.17.0") {
                    if (!contains(path, "monitors")) {
                        CArchive readArchive(READING_ARCHIVE);
                        if (readArchive.Lock(path, modeReadOnly, LOCK_NOWAIT)) {
                            bool isTrace = contains(path, "/traces/");
                            bool isRecon = contains(path, "/recons/");
                            bool isNames = contains(path, "/names/");
                            checker->needs = readArchive.needsUpgrade(isTrace || isRecon || isNames);
                            LOG_INFO(bBlack, "    Checking '", relativize(path), "'", cOff, "\r");
                            readArchive.Release();
                        }
                    }
                } else if (w == "0.30.0") {
                    if (contains(path, ".acct.bin")) {
                        checker->needs = true;
                    }
                }
            }
        }
        if (checker->needs) {
            checker->msg = path;
            return false;  // quit after we find the first one that needs migration
        }
    }
    return !shouldQuit();
}

//--------------------------------------------------------------------------------
bool COptions::handle_migrate_test(const CStringArray& cachePaths) {
    LOG_INFO("Checking caches at ", bBlue, cacheFolder, cOff);
    for (auto cache : cachePaths) {
        CMigrationChecker checker(cache, "0.17.0,0.30.0");
        forEveryFileInFolder(cache, needsMigration, &checker);  // will quit early if it finds a migrate
        if (checker.needs) {
            LOG_WARN(bBlue, "  ", relativize(cache), cYellow, " needs a migration at ", bBlack, relativize(checker.msg),
                     cOff);
        } else {
            LOG_INFO(bBlue, "  ", relativize(cache), cOff, " is up to date.", string_q(70, ' '));
        }
    }

    return false;
}
