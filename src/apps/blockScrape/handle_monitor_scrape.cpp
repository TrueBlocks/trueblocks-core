/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
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
        os << "acctExport ";
        os << (cache_txs ? "--cache_txs " : "");
        os << (cache_traces ? "--cache_traces " : "");
        os << (load.empty() ? "" : "--load " + load);
        os << (verbose ? ("--verbose " + uint_2_Str(verbose)) : "") << " ";
        os << monitor.address << " --freshen";
        LOG_INFO("Calling: " + os.str() + string_q(40, ' '));
        if (system(os.str().c_str()) != 0) {
            defaultQuitHandler(1);
            return false;
        }

        string_q unused;
        state = getCurrentState(unused);
        usleep(50000);  // allows user to get a control+c in edgewise
    }

    return !shouldQuit();
}

//---------------------------------------------------------------------------
bool prepareMonitors(const string_q& path, void* data) {
    if (!endsWith(path, ".acct.bin"))  // we only want to process monitor files
        return true;

    CMonitor m;
    m.address = substitute(substitute(path, m.getMonitorPath(""), ""), ".acct.bin", "");
    if (isAddress(m.address)) {
        m.needsRefresh = false;
        CMonitorArray* array = (CMonitorArray*)data;  // NOLINT
        array->push_back(m);
    }

    return true;
}
