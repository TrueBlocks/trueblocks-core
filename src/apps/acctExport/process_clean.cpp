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

//---------------------------------------------------------------
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

//---------------------------------------------------------------
bool COptions::process_clean(void) {
    CMonitor m;
    cout << "[";
    bool ret = forEveryFileInFolder(m.getPathToMonitor(m.address, false), cleanMonitorFile, NULL);
    cout << "]";
    return ret;
}
