/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"
#include "question.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_config(void) {
    string_q monitor = toLower(nextTokenClear(remainder, '|'));
    ostringstream os;
    os << "cd " << blockCachePath("monitors/" + monitor + "/") << " ; ";
    if (contains(remainder, "edit"))
        os << "nano config.toml";
    else
        os << "cat config.toml";
    if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings
    return true;
}
