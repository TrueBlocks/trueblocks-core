/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//-------------------------------------------------------------------------
int main(int argc, const char *argv[]) {
    etherlib_init("binary", quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        if (options.hasAddrs) {
            ASSERT(options.hasBlocks);
            options.blocks.forEveryBlockNumber(handle_addrs, &options);

        } else {

            if (options.isRaw) {
                options.blocks.forEveryBlockNumber(handle_raw, &options);

            } else if (options.isStats) {
                options.blocks.forEveryBlockNumber(handle_stats, &options);

            } else if (options.isCheck) {
                options.blocks.forEveryBlockNumber(handle_check, &options);

            } else if (options.isRewrite) {
                options.blocks.forEveryBlockNumber(handle_rewrite, &options);

            } else {
                options.blocks.forEveryBlockNumber(handle_visit, &options);

            }
        }
    }

    return 0;
}
