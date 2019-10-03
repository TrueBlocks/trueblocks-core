/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-------------------------------------------------------------------------
int main(int argc, const char *argv[]) {

    nodeNotRequired();
    acctlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    bool once = true;
    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        if (options.isListing) {
            if (once)
                cout << exportPreamble(options.exportFmt, expContext().fmtMap["header"], GETRUNTIME_CLASS(CStatus));
            options.handle_listing(cout);

        } else if (options.isConfig) {
            if (once)
                cout << exportPreamble(options.exportFmt, expContext().fmtMap["header"], GETRUNTIME_CLASS(CConfiguration));
            options.handle_config(cout);

        } else {
            if (once)
                cout << exportPreamble(options.exportFmt, expContext().fmtMap["header"], GETRUNTIME_CLASS(CStatus));
            options.handle_status(cout);

        }
        once = false;
    }
    cout << exportPostamble(options.exportFmt, expContext().fmtMap["meta"]);

    acctlib_cleanup();
    return 0;
}
