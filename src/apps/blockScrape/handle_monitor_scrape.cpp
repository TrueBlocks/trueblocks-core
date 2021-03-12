/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//------------------------------------------------------------------------------------------------
bool COptions::scrape_monitors(void) {
    ENTER("scrape_monitors");

    CMonitor m;
    CMonitorArray monitors;
    forEveryFileInFolder(m.getMonitorPath("") + "*", prepareMonitors, &monitors);

    for (auto monitor : monitors) {
        // We check frequently if the user has told us to quit (either by sending
        // a direct command to pause or by hitting control+C)
        if (state == STATE_STOPPED || shouldQuit())
            break;

        ostringstream os;
        os << "acctExport " << monitor.address << " --freshen";
        LOG4("Calling: ", os.str() + string_q(40, ' ') + "\r");
        // clang-format off
        if (system(os.str().c_str())) {}  // Don't remove cruft. Silences compiler warnings
        // clang-format on

        string_q unused;
        state = getCurrentState(unused);
        usleep(50000);  // allows user to get a control+c in edgewise
    }

    return true;
}

//---------------------------------------------------------------------------
bool prepareMonitors(const string_q& path, void* data) {
    if (!endsWith(path, ".acct.bin"))  // we only want to process monitor files
        return true;

    CMonitor m;
    m.address = substitute(substitute(path, m.getMonitorPath(""), ""), ".acct.bin", "");
    if (isAddress(m.address)) {
        m.cntBefore = m.getRecordCount();
        m.needsRefresh = false;
        CMonitorArray* array = (CMonitorArray*)data;  // NOLINT
        array->push_back(m);
    }

    return true;
}

// TODO(tjayrush): If the block scraper 'touched' each monitor that needs updating every
// TODO(tjayrush): time something changed, we could skip over anything that hasn't changed
// TODO(tjayrush): when we do an update
