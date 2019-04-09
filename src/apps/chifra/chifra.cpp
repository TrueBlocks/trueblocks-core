/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"
#include "question.h"

//--------------------------------------------------------------
int main(int argc, const char *argv[]) {
    nodeNotRequired(); // not every command needs a node
    acctlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

             if (options.mode == "list")    return !options.handle_list();
        else if (options.mode == "export")  return !options.handle_export();
        else if (options.mode == "init")    return !options.handle_init();
        else if (options.mode == "seed")    return !options.handle_seed();
        else if (options.mode == "daemon")  return !options.handle_daemon();
        else if (options.mode == "scrape")  return !options.handle_scrape();
        else if (options.mode == "ls")      return !options.handle_ls();
        else if (options.mode == "rm")      return !options.handle_rm();
        else if (options.mode == "names")   return !options.handle_names();
        else if (options.mode == "config")  return !options.handle_config();
        else cerr << "Should not happen.";
    }

    acctlib_cleanup();
    return 0;
}
