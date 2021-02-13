/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
extern bool removeAbi(const string_q& path, void *data);
int main(int argc, const char* argv[]) {
    acctlib_init(quickQuitHandler);

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
                                               : (options.statements ? GETRUNTIME_CLASS(CReconciliation)->m_ClassName
                                                                     : GETRUNTIME_CLASS(CTransaction)->m_ClassName)))));
        // clang-format on

        if (once)
            cout << exportPreamble(expContext().fmtMap["header"], options.className);

        if (options.loadAllAppearances()) {
            if (options.count) {
                bool first = true;
                for (auto cnt : options.counts) {
                    if (shouldQuit())
                        break;
                    cout << ((isJson() && !first) ? ", " : "");
                    cout << cnt.Format() << endl;
                    first = false;
                }

            } else if (options.appearances) {
                options.hanlde_appearances();

            } else if (options.receipts) {
                options.handle_receipts();

            } else if (options.traces) {
                options.handle_traces();

            } else if (options.logs) {
                options.handle_logs();

            } else if (options.statements) {
                options.handle_statements();

            } else {
                ASSERT(accounting);
                //for (int i = 0 ; i < 100 ; i++) {
                //    forEveryFileInFolder(getCachePath("abis/"), removeAbi, nullptr);
                    options.handle_accounting();
                //}
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
        if (options.abi_spec.nInterfaces() == 0) {
            HIDE_FIELD(CMonitor, "abi_spec");
        }
        os << ", \"accountedFor\": " << options.monitors[0] << endl;
    }
    expContext().fmtMap["meta"] += os.str();

    cout << exportPostamble(options.errors, expContext().fmtMap["meta"]);

    if (!isTestMode() && !options.freshen && !options.count && !options.accounting && !options.logs &&
        !options.statements) {
        ostringstream oss;
        oss << "Exported " << padNum6T(options.nProcessed) << " ";
        oss << (!options.className.empty() ? (plural(options.className) + " from ") : "of ");
        oss << padNum6T(options.nTransactions) << " transactions for address " << options.monitors[0].address;
        LOG_INFO(oss.str());
    }

    acctlib_cleanup();
    return 0;
}

bool removeAbi(const string_q& path, void *data) {
    if (endsWith(path, "/")) {
        return forEveryFileInFolder(path + "*", removeAbi, data);
    } else {
        ::remove(path.c_str());
    }
    return true;
}
