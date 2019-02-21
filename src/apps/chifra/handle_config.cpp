/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"
#include "question.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_config(void) {

    string_q subCmd = toLower(nextTokenClear(remainder, '|'));
    if (subCmd != "edit" && subCmd != "show")
        return usage("config mode must be either 'edit' or 'show'. Quitting...");

    while (!remainder.empty()) {
        string_q monitor = toLower(nextTokenClear(remainder, '|'));
        string_q path = blockCachePath("monitors/" + monitor + "/config.toml");
        if (!fileExists(path))
            return usage("File '" + path + "' not found. Quitting...");

        if (subCmd == "edit") {
            editFile(path);

        } else {
            ASSERT(subCmd == "show");
            ostringstream os;
            os << "cat " << path;
            if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings
        }
    }

    return true;
}
