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
    rawData = reinterpret_cast<char*>(malloc(sizeInBytes + (2 * asciiAppearanceSize)));  // extra room
    if (!rawData) {
        stage.Release();
        LOG_WARN("Could not allocate memory for data.");
        return !shouldQuit();
    }
    bzero(rawData, sizeInBytes + (2 * asciiAppearanceSize));

    size_t nRead = stage.Read(rawData, sizeInBytes, sizeof(char));
    if (nRead != sizeInBytes) {
        LOG_WARN("Could not read entire file.");
        return !shouldQuit();
    }

    size_t nRecords = fileSize(path) / asciiAppearanceSize;
    // stats.nRecords += nRecords;
    // cerr << "Unsorted: " << endl << rawData << endl;
    qsort(rawData, nRecords, asciiAppearanceSize, sortRecord);
    // cout << "Sorted: " << endl << rawData << endl;
    endOfData = rawData + sizeInBytes;

    for (size_t mo = 0; mo < allMonitors.size() && !shouldQuit(); mo++) {
        CMonitor* monitor = &allMonitors[mo];
        char search[70];
        strncpy(search, monitor->address.c_str(), monitor->address.size());
        search[monitor->address.size()] = '\0';
        char* found = (char*)bsearch(search, rawData, nRecords, asciiAppearanceSize, findAppearance);
        if (!found) {
            LOG_PROGRESS(SCANNING, fileRange.first, listRange.second, " stage miss");
        } else {
            stats.nStageHits++;
            char* ptr = found;
            while (ptr > rawData) {
                // Go backwards until we hit either the top of the file or the record
                // before the first record in the file with the address we're interested
                // in, then skip ahead one to get to the first record.
                ptr -= asciiAppearanceSize;
                if (findAppearance(search, ptr))
                    ptr = rawData;
                else
                    found = ptr;
            }

            while (found < endOfData) {
                char* pos = &found[58];
                *pos = '\0';
                string_q s = found;
                address_t addr = nextTokenClear(s, '\t');
                if (startsWith(addr, monitor->address)) {
                    CAppearance_mon app;
                    app.blk = (uint32_t)str_2_Uint(nextTokenClear(s, '\t'));
                    app.txid = (uint32_t)str_2_Uint(s);
                    monitor->apps.push_back(app);
                    found += asciiAppearanceSize;
                } else {
                    found = endOfData;
                }
            }

            found = endOfData;
            LOG_PROGRESS(SCANNING, fileRange.first, listRange.second, " stage hit");
        }
    }

    stage.Release();
    free(rawData);
    rawData = NULL;

    return !shouldQuit();
}

//---------------------------------------------------------------
int sortRecord(const void* v1, const void* v2) {
    const char* s1 = (const char*)v1;
    const char* s2 = (const char*)v2;
    return strncmp(s1, s2, asciiAppearanceSize);
}

//----------------------------------------------------------------
int findAppearance(const void* v1, const void* v2) {
    // char str[50];
    // bzero(str, 50);
    // cerr << strncpy(str, (char*)v1, 42) << " --> ";
    //      << strncpy(str, (char*)v2, 42) << ": "
    //      << strncmp(s1, s2, 42) << endl;
    const char* s1 = (const char*)v1;
    const char* s2 = (const char*)v2;
    return strncmp(s1, s2, 42);
}
