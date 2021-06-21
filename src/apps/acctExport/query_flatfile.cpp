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
bool COptions::queryFlatFile(const string_q& path, bool sorted) {
    char* rawData = NULL;

    CArchive stage(READING_ARCHIVE);
    if (!stage.Lock(path, modeReadOnly, LOCK_NOWAIT)) {
        LOG_WARN("Could not open index file " + path + ".");
        return !shouldQuit();
    }

    size_t sz = fileSize(path);
    rawData = reinterpret_cast<char*>(malloc(sz + (2 * 59)));  // extra room
    if (!rawData) {
        stage.Release();
        LOG_WARN("Could not allocate memory for data.");
        return !shouldQuit();
    }
    bzero(rawData, sz + (2 * 59));

    size_t nRead = stage.Read(rawData, sz, sizeof(char));
    if (nRead != sz) {
        LOG_WARN("Could not read entire file.");
        return !shouldQuit();
    }

    size_t nRecords = fileSize(path) / 59;
    // stats.nRecords += nRecords;
    qsort(rawData, nRecords, 59, sortByAddress);

    for (size_t mo = 0; mo < allMonitors.size() && !shouldQuit(); mo++) {
        CMonitor* monitor = &allMonitors[mo];
        if (!monitor->openForWriting(monitor->isStaging)) {
            delete rawData;
            rawData = NULL;
            LOG_WARN("Could not open monitor file for " + monitor->address + ".");
            return !shouldQuit();
        }
        char search[70];
        strncpy(search, monitor->address.c_str(), monitor->address.size());
        search[66] = '\0';
        char* found = (char*)bsearch(search, rawData, nRecords, 59, findAppearance2);
        if (found) {
            char* pos = &found[58];
            *pos = '\0';
            stats.nStageHits++;
            string_q s = found;
            nextTokenClear(s, '\t');
            CAppearance_mon app;
            app.blk = (uint32_t)str_2_Uint(nextTokenClear(s, '\t'));
            app.txid = (uint32_t)str_2_Uint(found);
            monTmp.push_back(app);
        } else {
            cerr << monitor->address << " not found on stage" << endl;
        }
        // if (tmp.size()) {
        //     lockSection();
        //     monitor->writeMonitorArray(tmp);
        //     monitor->writeMonitorLastBlock(fileRange.first + 1, monitor.isStaging);
        //     unlockSection();
        // }
    }

    if (!isTestMode())
        LOG_PROGRESS("Scanning", fileRange.first, listRange.second,
                     " stage " + string_q(monTmp.size() ? " hit" : " miss"));

    stage.Release();
    delete rawData;
    rawData = NULL;

    return !shouldQuit();
}

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
