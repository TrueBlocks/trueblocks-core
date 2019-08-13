/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-------------------------------------------------------------------------
int main(int argc, const char *argv[]) {
    acctlib_init(defaultQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;
        options.getStatus(cout);
    }

    acctlib_cleanup();

    return 0;
}
