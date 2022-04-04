// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package monitorsPkg

func (opts *MonitorsOptions) HandleClean() error {
	return nil
}

/*
bool cleanMonitorFile(const string_q& path, void* data) {
    if (endsWith(path, '/')) {
        forEveryFileInFolder(path + "*", cleanMonitorFile, data);

    } else {
        if (isMonitorFilePath(path)) {
            if (isTestMode()) {
                CStringArray testes = {"0x001d14804b399c6ef80e64576f657660804fec0b",
                                       "0x0029218e1dab069656bfb8a75947825e7989b987"};
                for (auto t : testes) {
                    if (contains(path, t)) {
                        return true;
                    }
                }
                return false;
            }

            CMonitor m;
            size_t sizeThen = m.getRecordCnt(path);
            if (!sizeThen)
                return !shouldQuit();

            if (!m.removeDuplicates(path))
                return false;

            static bool first = true;
            size_t sizeNow = m.getRecordCnt(path);
            if (verbose || sizeThen != sizeNow) {
                if (!first)
                    cout << ",";
                first = false;
                cout << "{ ";
                cout << "\"path\": \"" << substitute(path, m.getPathToMonitor("", false), "$CACHE/") << "\", ";
                cout << "\"sizeThen\": " << sizeThen << ", ";
                cout << "\"sizeNow\": " << sizeNow;
                if (sizeThen > sizeNow)
                    cout << ", \"dupsRemoved\": " << (sizeThen - sizeNow);
                cout << " }" << endl;
            }
        }
    }

    return !shouldQuit();
}
bool COptions::process_clean(void) {
    CMonitor m;
    cout << "[";
    bool ret = forEveryFileInFolder(m.getPathToMonitor(m.address, false), cleanMonitorFile, NULL);
    cout << "]";
    return ret;
}
// TODO: BOGUS - Do we need to use monitors/staging or can we build it in memory?
//----------------------------------------------------------------
bool CMonitor::removeDuplicates(const string_q& path) {
    if (!isMonitorFilePath(path))
        return false;
    CStringArray parts;
    explode(parts, path, '/');
    address = substitute(parts[parts.size() - 1], "mon.bin", "");

    if (!readAppearances(nullptr, nullptr)) {
        LOG_WARN("Could load monitor for address ", address);
        return false;
    }
    sort(apps.begin(), apps.end());

    CAppearance_mon prev;
    bool hasDups = false;
    for (auto a : apps) {
        if (a.blk == prev.blk && a.txid == prev.txid) {
            hasDups = true;
            break;
        }
        prev = a;
    }
    if (!hasDups)
        return true;

    CAppearanceArray_mon deduped;
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

    return true;
}
*/
