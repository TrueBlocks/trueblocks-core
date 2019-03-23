/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//----------------------------------------------------------------------------------
int main(int argc, const char *argv[]) {
    etherlib_init("local",  defaultQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        cerr << bGreen << "Scraping new blocks..." << "\n" << cOff;
        if (!handle_freshen(options))
            cerr << "\tThe tool ended with an error.";
        cerr << bGreen << "...done\n" << cOff;
        cout << scraperStatus();
    }

    etherlib_cleanup();
    return 0;
}
