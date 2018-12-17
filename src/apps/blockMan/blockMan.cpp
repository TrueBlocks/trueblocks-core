/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//----------------------------------------------------------------------------------
int main(int argc, const char *argv[]) {

    etherlib_init("binary", quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        cerr << bGreen << "Starting scan...\n" << cOff;

        if (options.modes & OP_LIST || options.modes & OP_STATS)
            handle_list(options);

        if (options.modes & OP_CHECK)
            handle_check(options);

        cerr << bGreen << "...done\n" << cOff;
    }

    etherlib_cleanup();
}
