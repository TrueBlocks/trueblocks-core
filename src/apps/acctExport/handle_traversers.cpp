/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
bool COptions::handle_traversers(void) {
    CDynamicTraverser lib(load);
    if (!lib.is_valid())
        return usage("Dynamic library " + load + " was found but is not valid.");

    freshenTimestamps(getBlockProgress().client);
    loadTimestamps(&expContext().tsMemMap, expContext().tsCnt);
    auto libFactory = lib.get_function<CTraverser*(void)>("makeTraverser");
    CTraverser* trav = libFactory();
    if (trav->dataFunc == noopFunc)
        trav->dataFunc = loadTx_Func;
    trav->travRange = blockRange;
    for (auto monitor : allMonitors) {
        getNamedAccount(monitor, monitor.address);
        trav->monitorMap[monitor.address] = monitor;
    }
    for (auto monitor : allMonitors) {
        trav->curMonitor = &monitor;
        curMonitor = &monitor;
        monApps.clear();
        monitor.loadAppsFromPath(monApps, "", visitOnLoad, this);
        if (reversed)  // TODO(tjayrush): remove this comment once account works backwardly
            sort(monApps.begin(), monApps.end(), sortMonitoredAppearanceReverse);
        trav->traverse(monApps, this);
    }

    return true;
}
