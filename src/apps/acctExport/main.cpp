/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
int main(int argc, const char *argv[]) {
    acctlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    bool once = true;
    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        CRuntimeClass *pClass = (options.doTraces ? GETRUNTIME_CLASS(CTrace) :
                                 (options.doLogs ? GETRUNTIME_CLASS(CLogEntry) :
                                  (options.doBalances ? GETRUNTIME_CLASS(CEthState) :
                                   GETRUNTIME_CLASS(CTransaction))));
        if (once)
            cout << exportPreamble(options.exportFmt, expContext().fmtMap["header"], pClass);

        options.loadAllAppearances();
        if (options.doBalances) {
            options.exportBalances();
        } else {
            options.exportData();
        }

        once = false;
    }
    cout << exportPostamble(options.exportFmt, expContext().fmtMap["meta"]);

    acctlib_cleanup();
    return 0;
}
