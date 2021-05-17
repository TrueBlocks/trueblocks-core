/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//----------------------------------------------------------------
int main(int argc, const char* argv[]) {
    nodeNotRequired();
    etherlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    bool once = true;
    for (auto command : options.commandLines) {
        string_q orig = command;
        if (!options.parseArguments(command))
            return 0;

        if (once)
            cout << exportPreamble(expContext().fmtMap["header"], "CPinnedChunk");
        doCommand("ethscan.py " + command);
        once = false;
    }

    cout << exportPostamble(options.errors, expContext().fmtMap["meta"]);
    etherlib_cleanup();

    return 0;
}
