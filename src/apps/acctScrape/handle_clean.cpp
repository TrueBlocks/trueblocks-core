/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "acctlib.h"
#include "options.h"

//---------------------------------------------------------------
bool visitFile(const string_q& path, void* data) {
    ENTER("visitFile");

    if (endsWith(path, '/')) {
        forEveryFileInFolder(path + "*", visitFile, data);
    } else {
        if (endsWith(path, "acct.bin")) {
            size_t sizeThen = fileSize(path);
            size_t nRecords = (fileSize(path) / sizeof(CAppearance_base));
            if (!nRecords)
                EXIT_NOMSG(true);

            CAppearance_base* buffer = new CAppearance_base[nRecords];
            if (!buffer)
                EXIT_NOMSG(true); // continue anyway

            bzero((void*)buffer, nRecords * sizeof(CAppearance_base));
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
            for (size_t i = 0; i < nRecords ; i++) {
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
            cout << path << ": " << sizeThen << " - " << fileSize(path) << endl;
        }
    }
    return true;
}

//---------------------------------------------------------------
bool COptions::handle_clean(void) {
    CMonitor m;
    return forEveryFileInFolder(m.getMonitorPath(""), visitFile, NULL);
}
