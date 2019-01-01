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
                options.blocks.forEveryBlockNumber(handle_raw, NULL);

            } else if (options.isStats) {
                CStatistics stats(options);
                options.blocks.forEveryBlockNumber(handle_stats, &stats);
                stats.report(stats.lastMarker + stats.options.bucketSize);

            } else if (options.isCheck) {
                options.blocks.forEveryBlockNumber(handle_check, &options);
                if (!verbose)
                    cout << "\n";

            } else {
                ASSERT(options.isReWrite);
                options.blocks.forEveryBlockNumber(handle_visit, &options);
            }
        }
    }

    return 0;
}

//-------------------------------------------------------------------------
bool accumAddrs(const CAddressAppearance& item, void *data) {
    if (isZeroAddr(item.addr) || !data)
        return true;
    CAddressArray *array = (CAddressArray*)data;
    array->push_back(item.addr);
    return true;
}

//----------------------------------------------------------------
// Return 'true' if we want the caller NOT to visit the traces of this transaction
bool transFilter(const CTransaction *trans, void *data) {
    // TODO: Use an option here for deep trace
    if (!ddosRange(trans->blockNumber))
        return false;
    return (getTraceCount(trans->hash) > 250);
}
