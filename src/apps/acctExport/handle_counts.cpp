/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
bool COptions::exportCounts(void) {
    ENTER8("exportCounts");

    bool isJson = (exportFmt == JSON1 || exportFmt == API1 || exportFmt == NONE1);
    if (isJson)
        cout << "[";

    bool first = true;
    for (auto monitor : monitors) {
        size_t count = fileSize(getMonitorPath(monitor.address)) / sizeof(CAppearance_base);
        switch (exportFmt) {
            case NONE1:
            case JSON1:
            case API1:
                if (!first)
                    cout << ",";
                cout << "{ \"address\": \"" << monitor.address << "\", \"count\": " << count << "}" << endl;
                first = false;
                break;
            case TXT1:
                cout << monitor.address << "\t" << count << endl;
                break;
            case CSV1:
                cout << "\"" << monitor.address << "\", \"" << count << endl;
                break;
        }
    }

    if (isJson)
        cout << "]";

    EXIT_NOMSG8(true);
}
