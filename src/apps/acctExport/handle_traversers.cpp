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

    auto libFactory = lib.get_function<CTraverser*(void)>("makeTraverser");
    LOG_INFO(bBlue, "Instantiating traverser", cOff);
    CTraverser* trav = libFactory();
    if (trav->dataFunc == noopFunc)
        trav->dataFunc = loadTx_Func;
    trav->exportRange = exportRange;
    for (auto monitor : allMonitors) {
        findName(monitor.address, monitor);
        trav->monitorMap[monitor.address] = monitor;
    }

    for (auto monitor : allMonitors) {
        LOG_INFO(bBlue, "Starting traversal of ", monitor.address, cOff);
        trav->curMonitor = &monitor;
        curMonitor = &monitor;
        monitor.loadAppearances(visitOnLoad, this);
        if (reversed)  // TODO(tjayrush): remove this comment once account works backwardly
            sort(monitor.apps.begin(), monitor.apps.end(), sortMonitoredAppearanceReverse);
        trav->traverse(monitor.apps, this);
    }

    return true;
}
