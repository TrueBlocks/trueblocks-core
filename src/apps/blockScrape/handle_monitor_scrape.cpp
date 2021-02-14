/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//------------------------------------------------------------------------------------------------
bool COptions::scrape_monitors(void) {
    ENTER("scrape_monitors");

    CMonitorArray monitors;
    CMonitor m;
    // Catch the monitors addresses up to the scraper if in --deamon mode
    forEveryFileInFolder(m.getMonitorPath("") + "*", prepareMonitors, &monitors);
    if (!freshen_internal(monitors, getEnvStr("FRESHEN_FLAGS")))
        EXIT_FAIL("'chifra " + mode + "' returns false");

    for (auto monitor : monitors) {
        if (state == STATE_STOPPED || shouldQuit())
            break;
        ostringstream os;
        os << "acctExport " << monitor.address << " --freshen";
        LOG_INFO("Calling: ", os.str(), string_q(40, ' '), "\r");
        // clang-format off
        if (system(os.str().c_str())) {}  // Don't remove cruft. Silences compiler warnings
        // clang-format on
        state = getCurrentState();
        usleep(50000);
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
