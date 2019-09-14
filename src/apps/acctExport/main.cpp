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

        options.className =
        (options.doAppearances ? "CAppearance" :
         (options.doTraces ? GETRUNTIME_CLASS(CTrace)->m_ClassName :
          (options.doLogs ? GETRUNTIME_CLASS(CLogEntry)->m_ClassName :
           (options.doBalances ? GETRUNTIME_CLASS(CEthState)->m_ClassName :
            (options.count_only ? "CCounts" : GETRUNTIME_CLASS(CTransaction)->m_ClassName)))));

        if (once)
            cout << exportPreamble(options.exportFmt, expContext().fmtMap["header"], options.className);

        if (options.count_only) {
            options.exportCounts();

        } else if (options.doBalances) {
            options.loadAllAppearances(); // allow the balance query to continue even with no appearances
            options.exportBalances();
        } else {
            if (!options.loadAllAppearances())
                return options.usage("Nothing to export.");
            options.exportData();
        }

        once = false;
    }
    cout << exportPostamble(options.exportFmt, expContext().fmtMap["meta"]);

    if (!options.freshen_only && !options.count_only) {
        cerr << "exported " << options.nExported << " ";
        if (!options.className.empty())
            cerr << options.className << " from ";
        else
            cerr << "of ";
        cerr << options.items.size() << " transactions" << string_q(45,' ') << "\n";
        cerr.flush();
    }

    acctlib_cleanup();
    return 0;
}
