/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------
bool visitMonitors(const string_q& path, void* data) {
    if (!endsWith(path, ".acct.bin"))
        return !shouldQuit();

    CMonitor monitor;
    monitor.address = substitute(substitute(path, monitor.getMonitorPath(""), ""), ".acct.bin", "");

    COptions* opt = (COptions*)data;
    if (opt->state == STATE_STOPPED || shouldQuit())
        return false;

    ostringstream os;
    os << "acctExport ";
    os << (opt->cache_txs ? "--cache_txs " : "");
    os << (opt->cache_traces ? "--cache_traces " : "");
    os << (verbose ? ("--verbose " + uint_2_Str(verbose)) : "") << " ";
    os << "--freshen ";
    os << "--first_block " << monitor.getLastBlockInMonitor() << " ";
    os << monitor.address;
    if (opt->load.empty())
        LOG_INFO("Calling: " + substitute(os.str(), "acctExport", "chifra export") + string_q(40, ' '));
    else
        os << "--load " + opt->load;

    if (system(os.str().c_str()) != 0)
        return false;

    usleep(10000);  // allows user to get a control+c in edgewise

    string_q unused;
    opt->state = opt->getCurrentState(unused);

    return (opt->state != STATE_STOPPED && !shouldQuit());
}

//------------------------------------------------------------------------------------------------
bool COptions::scrape_monitors(void) {
    CMonitor m;
    return forEveryFileInFolder(m.getMonitorPath("") + "*", visitMonitors, this);
}
