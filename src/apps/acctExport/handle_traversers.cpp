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

//-----------------------------------------------------------------------
bool COptions::handle_traversers(void) {
    CDynamicTraverser lib(load);
    if (!lib.is_valid())
        return usage("Dynamic library " + load + " was found but is not valid.");
    if (!loadNames(true))
        return usage("Could not load names database.");

    auto libFactory = lib.get_function<CTraverser*(void)>("makeTraverser");
    LOG_INFO(bBlue, "Instantiating traverser", cOff);
    CTraverser* trav = libFactory();
    if (trav->dataFunc == noopFunc || trav->dataFunc == nullptr)
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
