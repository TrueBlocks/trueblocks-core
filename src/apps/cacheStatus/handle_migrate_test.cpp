/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

class CChecker {
  public:
    bool needs;
    string_q path;
    CChecker(const string_q& p) : needs(false), path(p) {
    }

  private:
    CChecker(void) = delete;
    CChecker(const CChecker&) = delete;
    CChecker& operator=(const CChecker&) = delete;
};

//--------------------------------------------------------------------------------
bool needsMigrate(const string_q& path, void* data) {
    CChecker* checker = (CChecker*)data;
    if (endsWith(path, "/")) {
        return forEveryFileInFolder(path + "*", needsMigrate, data);

    } else {
        string_q relative = substitute(path, getCachePath(""), "$CACHE/");
        if (endsWith(path, ".bin")) {
            CArchive archive(READING_ARCHIVE);
            if (archive.Lock(path, modeReadOnly, LOCK_NOWAIT)) {
                checker->needs = archive.needsUpgrade();
                LOG_INFO("  Checking ", cBlue, relative, cOff, " isCurrent: ", cBlue,
                         (checker->needs ? "false" : "true"), cOff, (checker->needs ? "" : "\r"));
                if (checker->needs || !shouldQuit())
                    return false;  // quit after we find the first one that needs migrate
            }
        } else {
            LOG_INFO("  Skipping ", cYellow, relative, cOff, "\r");
            cerr.flush();
        }
    }
    return !shouldQuit();
}

//--------------------------------------------------------------------------------
bool COptions::handle_migrate_test(void) {
    bool needs = false;
    for (auto cache : cachePaths) {
        string_q path = getCachePath(cache);
        LOG_INFO(cGreen, "Checking '$CACHES/", cache, "'", string_q(50, ' '), cOff);
        CChecker checker(path);
        forEveryFileInFolder(path, needsMigrate, &checker);  // will quit early if it finds a migrate
        if (checker.needs) {
            LOG_WARN("  Cache '$CACHES/", cache, "' needs a migration.");
            needs = true;
        }
    }

    return usage(string_q("Migrations are ") + (needs ? "" : "not ") + "needed.");
}
