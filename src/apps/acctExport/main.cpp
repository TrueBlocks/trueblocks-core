/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    acctlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    bool once = true;
    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        options.className =
            (options.count
                 ? GETRUNTIME_CLASS(CMonitorCount)->m_ClassName
                 : options.appearances
                       ? GETRUNTIME_CLASS(CAppearance)->m_ClassName
                       : (options.traces
                              ? GETRUNTIME_CLASS(CTrace)->m_ClassName
                              : (options.receipts ? GETRUNTIME_CLASS(CReceipt)->m_ClassName
                                                  : (options.logs ? GETRUNTIME_CLASS(CLogEntry)->m_ClassName
                                                                  : GETRUNTIME_CLASS(CTransaction)->m_ClassName))));

        if (once)
            cout << exportPreamble(expContext().fmtMap["header"], options.className);

        if (options.loadAllAppearances()) {
            if (options.count) {
                options.exportCounts();

            } else if (options.appearances) {
                options.exportAppearances();

            } else if (options.accounting) {
                options.exportAccounting();

            } else {
                options.exportData();
            }
        }

        once = false;
    }

    ostringstream os;
    os << ", \"start\": " << (isTestMode() ? "\"0xdeadbeef\"" : uint_2_Str(options.scanRange.first)) << endl;
    os << ", \"end\": " << (isTestMode() ? "\"0xdeadbeef\"" : uint_2_Str(options.scanRange.second)) << endl;
    if (!options.count && options.monitors.size() == 1) {
        options.getNamedAccount(options.monitors[0], options.monitors[0].address);
        HIDE_FIELD(CMonitor, "summaryStatement");
        if (options.monitors[0].abi_spec.interfaces.size() == 0) {
            HIDE_FIELD(CMonitor, "abi_spec");
        }
        os << ", \"accountedFor\": " << options.monitors[0] << endl;
    }
    expContext().fmtMap["meta"] += os.str();

    cout << exportPostamble(options.errors, expContext().fmtMap["meta"]);

    if (!isTestMode() && !options.freshen && !options.count) {
        ostringstream oss;
        oss << "exported " << padNum6T(options.nExported) << " ";
        oss << (!options.className.empty() ? (plural(options.className) + " from ") : "of ");
        oss << padNum6T(options.nTransactions) << " transactions for address " << options.monitors[0].address;
        LOG_INFO(oss.str());
    }

    acctlib_cleanup();
    return 0;
}
