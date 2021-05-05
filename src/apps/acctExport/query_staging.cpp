/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

extern int findAppearance2(const void* v1, const void* v2);
extern int sortByAddress(const void* v1, const void* v2);
extern bool isHit(char* s, const CMonitorArray& monitors);
//---------------------------------------------------------------
bool visitStagingIndexFiles(const string_q& path, void* data) {
    return !shouldQuit();
#if 0
    ENTER("visitStagingIndexFile");

    if (endsWith(path, "/")) {
        return forEveryFileInFolder(path + "*", visitStagingIndexFiles, data);

    } else {
        // There should only be a single file in the ./staging folder (other than
        // 0000000000-temp.txt which we skip). The file contains all blocks the scraper
        // has seen but has not yet consolidated. Here, we read the file which is
        // fixed length: [address, blockNum, txid (59 bytes)]. We read the entire file
        // into memory at once, sort it by address (the file is sorted by block and since
        // new blocks will be appended, there's no reason so sort it each time) and then
        // binary search for the address we're looking for. Should be super fast.
        COptions* options = reinterpret_cast<COptions*>(data);
        options->stats.nFiles++;

        if (!contains(path, "staging/0") || !endsWith(path, ".txt") || contains(path, "-temp")) {
            options->stats.nSkipped++;
            EXIT_NOMSG(!shouldQuit());
        }

        timestamp_t unused;
        options->fileRange.first = bnFromPath(path, options->fileRange.second, unused);
        ASSERT(unused != NOPOS && options->fileRange.first != NOPOS && options->fileRange.second != NOPOS);

        // Note that `start` and `end` options are ignored when scanning
        if (!rangesIntersect(options->listRange, options->fileRange)) {
            options->stats.nSkipped++;
            EXIT_NOMSG(!shouldQuit());
        }

        char* rawData = NULL;

        options->stats.nStageChecked++;

        CArchive stage(READING_ARCHIVE);
        if (!stage.Lock(path, modeReadOnly, LOCK_NOWAIT))
            EXIT_FAIL("Could not open index file " + path + ".");

        size_t sz = fileSize(path);
        rawData = reinterpret_cast<char*>(malloc(sz + (2 * 59)));  // extra room
        if (!rawData) {
            stage.Release();
            EXIT_FAIL("Could not allocate memory for data.");
        }
        bzero(rawData, sz + (2 * 59));

        size_t nRead = stage.Read(rawData, sz, sizeof(char));
        if (nRead != sz)
            EXIT_FAIL("Could not read entire file.");

        size_t nRecords = fileSize(path) / 59;
        options->stats.nRecords += nRecords;
        qsort(rawData, nRecords, 59, sortByAddress);

        CAppearanceArray_base items;
        for (size_t mo = 0; mo < options->allMonitors.size() && !shouldQuit(); mo++) {
            CMonitor* monitor = &options->allMonitors[mo];
            if (!monitor->openForWriting()) {
                delete rawData;
                rawData = NULL;
                EXIT_FAIL("Could not open cache file " + monitor->getMonitorPath(monitor->address, monitor->fm _mode) +
                          ".");
            }
            char search[70];
            strncpy(search, monitor->address.c_str(), monitor->address.size());
            search[66] = '\0';
            char* found = (char*)bsearch(search, rawData, nRecords, 59, findAppearance2);
            if (found) {
                char* pos = &found[58];
                *pos = '\0';
                cout << found << endl;
                options->stats.nStageHits++;
                CAppearance_base app;
                string_q s = found;
                nextTokenClear(s, '\t');
                app.blk = (uint32_t)str_2_Uint(nextTokenClear(s, '\t'));
                app.txid = (uint32_t)str_2_Uint(found);
                items.push_back(app);
            } else {
                cerr << monitor->address << " not found on stage" << endl;
            }
            if (items.size()) {
                lockSection();
                monitor->writeMonitorArray(items);
                monitor->writeLastBlockInMonitor(options->fileRange.first + 1);
                unlockSection();
            }
        }

        LOG_PROGRESS("Scanning", options->fileRange.first, options->listRange.second,
                     " stage " + string_q(items.size() ? " hit" : " miss"));

        stage.Release();
        delete rawData;
        rawData = NULL;

        EXIT_NOMSG(!shouldQuit());
    }
    ASSERT(0);  // should not happen
    return !shouldQuit();
#endif
}

#if 0
//---------------------------------------------------------------
int sortByAddress(const void* v1, const void* v2) {
    char* s1 = (char*)v1;
    char* s2 = (char*)v2;
    return strncmp(s1, s2, 59);
}

//---------------------------------------------------------------
bool isHit(char* s, const CMonitorArray& monitors) {
    for (auto m : monitors)
        if (startsWith(s, m.address))
            return true;
    return false;
}

//----------------------------------------------------------------
int findAppearance2(const void* v1, const void* v2) {
    return sortByAddress(v1, v2) == 0;
}
#endif
