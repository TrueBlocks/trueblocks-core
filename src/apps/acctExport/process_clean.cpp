/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------
address_t path_2_Addr(const string_q& path) {
    if (!endsWith(path, ".acct.bin"))
        return "";
    CStringArray parts;
    explode(parts, path, '/');
    // for (auto part : parts)
    //     cerr << "part: " << part << endl;
    // cerr << "end: " << parts[parts.size() - 1] << endl;
    return substitute(parts[parts.size() - 1], ".acct.bin", "");
}

//---------------------------------------------------------------
bool cleanMonitorFile(const string_q& path, void* data) {
    ENTER("visitFile");

    if (endsWith(path, '/')) {
        forEveryFileInFolder(path + "*", cleanMonitorFile, data);

    } else {
        if (endsWith(path, "acct.bin")) {
            if (isTestMode()) {
                string_q testes =
                    "|0x001d14804b399c6ef80e64576f657660804fec0b|"
                    "|0x0029218e1dab069656bfb8a75947825e7989b987|";
                if (!contains(testes, path_2_Addr(path)))
                    return true;
            }

            CMonitor m;
            size_t sizeThen = m.getRecordCnt(path);
            if (!sizeThen)
                EXIT_NOMSG(!shouldQuit());

            m.address = path_2_Addr(path);
            if (!m.loadAppearances(nullptr, nullptr))
                EXIT_FAIL("Could not open cache file.");
            sort(m.apps.begin(), m.apps.end());

            CAppearance_mon prev;
            CAppearanceArray_mon deduped;
            for (auto a : m.apps) {
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
            size_t sizeNow = m.getRecordCnt(path);
            cout << "{ ";
            cout << "\"path\": \"" << substitute(path, m.getMonitorPath("", false), "$CACHE/") << "\", ";
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
    bool ret = forEveryFileInFolder(m.getMonitorPath(m.address, false), cleanMonitorFile, NULL);
    cout << "]";
    return ret;
}
