/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-------------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    nodeNotRequired();
    acctlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    bool once = true;
    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        if (options.isConfig) {
            if (once)
                cout << exportPreamble(expContext().fmtMap["header"], GETRUNTIME_CLASS(CConfiguration));
            options.handle_config(cout);

        } else {
            if (once)
                cout << exportPreamble(expContext().fmtMap["header"], "");
            options.handle_status(cout);
        }
        once = false;
    }
    cout << exportPostamble(options.errors, expContext().fmtMap["meta"]);

    acctlib_cleanup();
    return 0;
}
