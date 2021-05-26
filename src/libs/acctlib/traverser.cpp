/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "traverser.h"

namespace qblocks {

//-----------------------------------------------------------------------
bool forEveryAppearance(const CTraverserArray& traversers, const CAppearanceArray_base& apps, void* data) {
    const COptionsBase* opt = (const COptionsBase*)data;

    // If we have nothing to do, return success
    if (!apps.size() || !traversers.size())
        return true;

    // for each traverser...
    for (auto trav : traversers) {
        if (shouldQuit())
            break;

        // Make sure we have something to work with...
        if (!trav.filterFunc || !trav.preFunc || !trav.postFunc || !trav.displayFunc || !trav.dataFunc) {
            LOG_ERR("traverser function is null.");
            quickQuitHandler(1);
        }

        // Prepare the export...
        if (!(*trav.preFunc)(&trav, data))
            return false;

        // For each appearance...
        for (trav.index = 0; trav.index < apps.size() && !shouldQuit(); trav.index++) {
            trav.app = &apps[trav.index];
            trav.trans = CTransaction();  // reset
            bool passedFilter = !trav.filterFunc || (*trav.filterFunc)(&trav, data);
            if (passedFilter) {
                if (trav.displayFunc) {
                    // We freshen at most 5,000 new transactions to stay responsive
                    if (opt->freshenOnly && trav.nProcessed > 5000)
                        break;

                    if (!(*trav.dataFunc)(&trav, data))
                        return ((*trav.postFunc)(&trav, data)) && false;

                    if (!(*trav.displayFunc)(&trav, data))
                        return ((*trav.postFunc)(&trav, data)) && false;
                }
            }
        }

        // Cleanup the export...
        if (!(*trav.postFunc)(&trav, data))
            return false;
    }

    return true;
};

//-----------------------------------------------------------------------
bool rangeFilter(CTraverser* trav, void* data) {
    const COptionsBase* opt = (COptionsBase*)data;
    if (trav->app->blk > opt->scanRange.second || trav->app->blk >= expContext().tsCnt || shouldQuit())
        return false;
    return inRange(blknum_t(trav->app->blk), opt->scanRange.first, opt->scanRange.second);
}

}  // namespace qblocks
