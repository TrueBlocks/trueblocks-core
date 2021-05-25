/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    pinlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    bool once = true;
    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        // clang-format off
        options.className =
            (options.count ? GETRUNTIME_CLASS(CMonitorCount)->m_ClassName
             : options.appearances
                 ? GETRUNTIME_CLASS(CAppearance)->m_ClassName
                 : (options.traces
                        ? GETRUNTIME_CLASS(CTrace)->m_ClassName
                        : (options.receipts
                               ? GETRUNTIME_CLASS(CReceipt)->m_ClassName
                               : (options.logs ? GETRUNTIME_CLASS(CLogEntry)->m_ClassName
                                               : GETRUNTIME_CLASS(CTransaction)->m_ClassName))));
        // clang-format on

        if (once)
            cout << exportPreamble(expContext().fmtMap["header"], options.className);

        CTraverser t(&options, cout, "");
        CAppearanceDisplay dapp(options.accountedFor, options.accountedForName, NOPOS, NOPOS);
        CTraverserArray traversers;
        if (options.appearances) {
            t.operation = "appearances";
            t.displayFunc = app_Display;
            t.postFunc = app_Post;
            t.dataFunc = noopFunc;
            traversers.push_back(t);

        } else if (options.receipts) {
            t.operation = "receipts";
            t.displayFunc = receipts_Display;
            traversers.push_back(t);

        } else if (options.traces) {
            t.operation = "traces";
            t.displayFunc = traces_Display;
            traversers.push_back(t);

        } else if (options.logs) {
            t.operation = "logs";
            t.preFunc = logs_Pre;
            t.displayFunc = logs_Display;
            traversers.push_back(t);

        } else {
            t.operation = "txs";
            t.filterFunc = (options.freshen ? noopFunc : rangeFilter);
            t.preFunc = (options.freshen ? noopFunc : acct_Pre);
            t.postFunc = post_Func;
            t.displayFunc = (options.freshen ? noopFunc : acct_Display);
            t.dataFunc = (options.freshen ? noopFunc : loadData);
            traversers.push_back(t);
        }

        forEveryAppearance(traversers, options.apps, &dapp);
        if (shouldQuit())
            break;

        once = false;
    }

    ostringstream os;
    os << ", \"start\": " << (isTestMode() ? "\"0xdeadbeef\"" : uint_2_Str(options.scanRange.first)) << endl;
    os << ", \"end\": " << (isTestMode() ? "\"0xdeadbeef\"" : uint_2_Str(options.scanRange.second)) << endl;
    if (!options.count && options.allMonitors.size() == 1) {
        options.getNamedAccount(options.allMonitors[0], options.accountedFor);
        if (options.abi_spec.nInterfaces() == 0) {
            HIDE_FIELD(CMonitor, "abi_spec");
        }
        os << ", \"accountedFor\": " << options.allMonitors[0] << endl;
    }
    expContext().fmtMap["meta"] += os.str();

    cout << exportPostamble(options.errors, expContext().fmtMap["meta"]);

    options.writePerformanceData();

    pinlib_cleanup();
    return 0;
}
