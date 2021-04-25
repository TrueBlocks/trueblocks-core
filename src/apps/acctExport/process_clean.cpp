/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "acctlib.h"
#include "options.h"

//---------------------------------------------------------------
bool cleanMonitorFile(const string_q& path, void* data) {
    ENTER("visitFile");

    if (endsWith(path, '/')) {
        forEveryFileInFolder(path + "*", cleanMonitorFile, data);

    } else {
        if (endsWith(path, "acct.bin")) {
            if (isTestMode()) {
                CMonitor m;
                if (path > m.getMonitorPath("0x9"))
                    return false;
            }

            size_t sizeThen = (fileSize(path) / sizeof(CAppearance_base));
            blknum_t nRecords = (fileSize(path) / sizeof(CAppearance_base));
            if (!nRecords)
                EXIT_NOMSG(!shouldQuit());

            CAppearance_base* buffer = new CAppearance_base[nRecords];
            if (!buffer)
                EXIT_NOMSG(!shouldQuit());  // continue anyway

            bzero((void*)buffer, nRecords * sizeof(CAppearance_base));  // NOLINT
            CArchive archiveIn(READING_ARCHIVE);
            if (!archiveIn.Lock(path, modeReadOnly, LOCK_NOWAIT)) {
                archiveIn.Release();
                delete[] buffer;
                EXIT_FAIL("Could not open cache file.");
            }
            archiveIn.Read(buffer, sizeof(CAppearance_base), nRecords);
            archiveIn.Release();

            CAppearanceArray_base apps;
            apps.reserve(nRecords);
            for (size_t i = 0; i < nRecords; i++) {
                apps.push_back(buffer[i]);
            }
            sort(apps.begin(), apps.end());
            delete[] buffer;

            CAppearance_base prev;
            CAppearanceArray_base deduped;
            for (auto a : apps) {
                if (a.blk != prev.blk || a.txid != prev.txid) {
                    deduped.push_back(a);
                }
                prev = a;
            }

            CArchive archiveOut(WRITING_ARCHIVE);
            archiveOut.Lock(path, modeWriteCreate, LOCK_WAIT);
            for (auto item : deduped)
                archiveOut << item.blk << item.txid;
            archiveOut.Release();

            static bool first = true;
            if (!first)
                cout << ",";
            first = false;
            CMonitor m;
            size_t sizeNow = (fileSize(path) / sizeof(CAppearance_base));
            cout << "{ ";
            cout << "\"path\": \"" << substitute(path, m.getMonitorPath(""), "$CACHE/") << "\", ";
            cout << "\"sizeThen\": " << sizeThen << ", ";
            cout << "\"sizeNow\": " << sizeNow;
            if (sizeThen > sizeNow)
                cout << ", \"dupsRemoved\": " << (sizeThen - sizeNow);
            cout << " }" << endl;
        }
    }

    return !shouldQuit();
}

//---------------------------------------------------------------
bool COptions::process_clean(void) {
    CMonitor m;
    cout << "[";
    bool ret = forEveryFileInFolder(m.getMonitorPath(""), cleanMonitorFile, NULL);
    cout << "]";
    return ret;
}
