/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"
#include "question.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_scrape(void) {
    ostringstream os;
    os << "scraper.py";
    if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings
    return true;
}
