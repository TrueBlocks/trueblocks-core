/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//--------------------------------------------------------------
int main(int argc, const char* argv[]) {
    nodeNotRequired();  // not every command needs a node
    acctlib_init(quickQuitHandler);

    ENTER("chifra");
    COptions options;
    if (!options.prepareArguments(argc, argv))
        RETURN(EXIT_FAILURE);

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            RETURN(EXIT_FAILURE);

        if (options.mode == "list" || options.mode == "export") {
            options.tool_flags = substitute(options.tool_flags, "--addrs", "");
            RETURN(options.handle_export())

        } else if (options.mode == "scrape") {
            RETURN(options.handle_scrape())

        } else {
            RETURN(options.handle_commands());
        }
    }

    acctlib_cleanup();
    EXIT_NOMSG(EXIT_SUCCESS);
}
