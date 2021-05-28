/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//----------------------------------------------------------------
int main(int argc, const char* argv[]) {
    nodeNotRequired();
    pinlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    bool once = true;
    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        pinlib_readManifest(options.pins);
        if (once)
            cout << exportPreamble(expContext().fmtMap["header"], "CPinnedChunk");

        if (options.init) {
            return options.handle_init();

        } else {
            options.handle_list();
        }
        once = false;
    }

    cout << exportPostamble(options.errors, expContext().fmtMap["meta"]);
    pinlib_cleanup();

    return 0;
}
