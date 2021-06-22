/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

extern int findAppearance(const void* v1, const void* v2);
extern int sortRecord(const void* v1, const void* v2);
//---------------------------------------------------------------
bool COptions::queryFlatFile(const string_q& path, bool sorted) {
    char* rawData = nullptr;
    char* endOfData = nullptr;

    CArchive stage(READING_ARCHIVE);
    if (!stage.Lock(path, modeReadOnly, LOCK_NOWAIT)) {
        LOG_WARN("Could not open index file " + path + ".");
        return !shouldQuit();
    }

    size_t sizeInBytes = fileSize(path);
    rawData = reinterpret_cast<char*>(malloc(sizeInBytes + (2 * 59)));  // extra room
    if (!rawData) {
        stage.Release();
        LOG_WARN("Could not allocate memory for data.");
        return !shouldQuit();
    }
    bzero(rawData, sizeInBytes + (2 * 59));

    size_t nRead = stage.Read(rawData, sizeInBytes, sizeof(char));
    if (nRead != sizeInBytes) {
        LOG_WARN("Could not read entire file.");
        return !shouldQuit();
    }

    size_t nRecords = fileSize(path) / 59;
    // stats.nRecords += nRecords;
    // cerr << "Unsorted: " << endl << rawData << endl;
    qsort(rawData, nRecords, 59, sortRecord);
    // cout << "Sorted: " << endl << rawData << endl;
    endOfData = rawData + sizeInBytes;

    for (size_t mo = 0; mo < allMonitors.size() && !shouldQuit(); mo++) {
        CMonitor* monitor = &allMonitors[mo];
        char search[70];
        strncpy(search, monitor->address.c_str(), monitor->address.size());
        search[monitor->address.size()] = '\0';
        char* found = (char*)bsearch(search, rawData, nRecords, 59, findAppearance);
        if (!found) {
            if (!isTestMode())
                LOG_PROGRESS("Scanning", fileRange.first, listRange.second, " stage miss");
        } else {
            stats.nStageHits++;
            // go backwards until we hit either the top of the file or the record before
            // the first record in the file with the address, then skip ahead one
            char* ptr = found;
            while (ptr > rawData) {
                ptr -= 59;
                if (findAppearance(search, ptr)) {
                    ptr = rawData;
                } else {
                    found = ptr;
                }
                // char str[100];
                // bzero(str, 100);
                // strncpy(str, found, 59);
                // cerr << str;
                // getchar();
            }

            while (found < endOfData) {
                char* pos = &found[58];
                *pos = '\0';
                string_q s = found;
                address_t addr = nextTokenClear(s, '\t');
                cerr << addr << ": " << s << endl;
                if (startsWith(addr, monitor->address)) {
                    CAppearance_mon app;
                    app.blk = (uint32_t)str_2_Uint(nextTokenClear(s, '\t'));
                    app.txid = (uint32_t)str_2_Uint(s);
                    monitor->apps.push_back(app);
                    found += 59;
                } else {
                    found = endOfData;
                }
            }
            found = endOfData;
            if (!isTestMode())
                LOG_PROGRESS("Scanning", fileRange.first, listRange.second, " stage hit");
        }
    }

    stage.Release();
    delete rawData;
    rawData = NULL;

    return !shouldQuit();
}

//---------------------------------------------------------------
int sortRecord(const void* v1, const void* v2) {
    const char* s1 = (const char*)v1;
    const char* s2 = (const char*)v2;
    return strncmp(s1, s2, 59);
}

//----------------------------------------------------------------
int findAppearance(const void* v1, const void* v2) {
    const char* s1 = (const char*)v1;
    const char* s2 = (const char*)v2;

    char str[50];
    bzero(str, 50);
    cerr << strncpy(str, (char*)v1, 42) << " --> " << strncpy(str, (char*)v2, 42) << ": " << strncmp(s1, s2, 42)
         << endl;

    return strncmp(s1, s2, 42);
}
