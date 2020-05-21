/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

extern string_q plural(const string_q& in);
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
            (options.appearances
                 ? GETRUNTIME_CLASS(CAppearance)->m_ClassName
                 : (options.balances
                        ? GETRUNTIME_CLASS(CEthState)->m_ClassName
                        : (options.traces
                               ? GETRUNTIME_CLASS(CTrace)->m_ClassName
                               : (options.receipts ? GETRUNTIME_CLASS(CReceipt)->m_ClassName
                                                   : (options.logs ? GETRUNTIME_CLASS(CLogEntry)->m_ClassName
                                                                   : GETRUNTIME_CLASS(CTransaction)->m_ClassName)))));

        if (once)
            cout << exportPreamble(expContext().fmtMap["header"], options.className);

        if (options.balances) {
            options.loadAllAppearances();  // allow the balance query to continue even with no appearances
            options.exportBalances();

        } else {
            if (options.loadAllAppearances())
                options.exportData();
        }

        if (options.isRedirected()) {
            once = true;
            cout << exportPostamble(options.errors, expContext().fmtMap["meta"]);
        } else {
            once = false;
        }
    }
    if (!options.isRedirected())
        cout << exportPostamble(options.errors, expContext().fmtMap["meta"]);

    if (!options.freshen)
        LOG_INFO("exported ", options.nExported, " ",
                 (!options.className.empty() ? (plural(options.className) + " from ") : "of "), options.items.size(),
                 " transactions", string_q(55, ' '));

    acctlib_cleanup();
    return 0;
}

//--------------------------------------------------------------------------------
string_q plural(const string_q& in) {
    return substitute(substitute(toLower(in).substr(1, 1000) + "s", "logentrys", "logs"), "ethstates", "balances");
}
