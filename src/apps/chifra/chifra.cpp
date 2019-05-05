/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"
#include "question.h"

#define RETURN(a) { bool ret = (a); if (!getEnvStr("TEST_MODE").empty()) { return 0; } else { return ret; } }
//--------------------------------------------------------------
int main(int argc, const char *argv[]) {
    nodeNotRequired(); // not every command needs a node
    acctlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        RETURN(1);

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            RETURN(1);

             if (options.mode == "list")      RETURN(options.handle_list())
        else if (options.mode == "export")    RETURN(options.handle_export())
        else if (options.mode == "seed")      RETURN(options.handle_seed())
        else if (options.mode == "daemon")    RETURN(options.handle_daemon())
        else if (options.mode == "scrape")    RETURN(options.handle_scrape())
        else if (options.mode == "ls")        RETURN(options.handle_ls())
        else if (options.mode == "rm")        RETURN(options.handle_rm())
        else if (options.mode == "functions") RETURN(options.handle_functions())
        else if (options.mode == "data")      RETURN(options.handle_data())
        else if (options.mode == "config")    RETURN(options.handle_config())
        else cerr << "Should not happen.";
    }

    acctlib_cleanup();
    return 0;
}

