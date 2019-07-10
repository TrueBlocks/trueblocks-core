/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "acctlib.h"
#include "options.h"

//----------------------------------------------------------------------------------
int main(int argc, const char *argv[]) {
    etherlib_init("local",  defaultQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (options.parseArguments(command)) {
#ifdef OLD_CODE
#else
verbose = 1;
#endif
            cerr << bGreen << "Scraping new blocks..." << "\n" << cOff;
            if (!handle_scrape(options))
                cerr << "\tThe tool ended with an error.";
            cerr << bGreen << "...done\n" << cOff;
        }
    }

    if (verbose)
        cerr << scraperStatus();

    etherlib_cleanup();

    return 0;
}
