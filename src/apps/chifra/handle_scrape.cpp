/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_scrape(void) {

    ostringstream os;
    // Note: The docker build actually moves scraper.docker.py to scraper.py, so this is only for testing
    if (flags == "docker")
        os << "scraper.docker.py";
    else
        os << "scraper.py";
    if (isTestMode())
        cout << os.str();
    else
        if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings

    return true;
}
