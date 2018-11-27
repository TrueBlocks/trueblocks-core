/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//-----------------------------------------------------------------------
int main(int argc, const char *argv[]) {

    etherlib_init(quickQuitHandler);

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    CStringArray commands;
    explode(commands, options.commandList, '\n');
    if (commands.empty()) commands.push_back("--noop");
    for (auto command : commands) {
        if (!options.parseArguments(command))
            return 0;

        loadData(options);
        exportData(options);
    }

    return 0;
}
