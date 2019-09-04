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

        string_q className =
        (options.doAppearances ? "CAppearance" :
         (options.doTraces ? GETRUNTIME_CLASS(CTrace)->m_ClassName :
          (options.doLogs ? GETRUNTIME_CLASS(CLogEntry)->m_ClassName :
           (options.doBalances ? GETRUNTIME_CLASS(CEthState)->m_ClassName : GETRUNTIME_CLASS(CTransaction)->m_ClassName))));
        if (once)
            cout << exportPreamble(options.exportFmt, expContext().fmtMap["header"], className);

        options.loadAllAppearances();
        if (options.doBalances) {
            options.exportBalances();
        } else {
            options.exportData();
        }

        once = false;
    }
    cout << exportPostamble(options.exportFmt, expContext().fmtMap["meta"]);

    if (!options.freshenOnly) {
        cerr << "exported " << options.nExported << " ";
        cerr << (options.doTraces ? "traces from " : (options.doLogs ? "logs from " : (options.doABIs ? "abis from " : "of ")));
        cerr << options.items.size() << " transactions" << string_q(45,' ') << "\n";
        cerr.flush();
    }

    acctlib_cleanup();
    return 0;
}
