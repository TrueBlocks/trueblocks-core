/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "traverser.h"

namespace qblocks {

//-----------------------------------------------------------------------
bool forEveryAppearance(const CTraverserArray& traversers, const CAppearanceArray_base& apps, void* data) {
    // If we have nothing to do, return success
    if (!apps.size() || !traversers.size())
        return true;

    // for each traverser...
    for (auto trav : traversers) {
        if (shouldQuit())
            break;
        if (!trav.execute(apps, data))
            return false;
    }

    return true;
};

//-----------------------------------------------------------------------
bool CTraverser::execute(const CAppearanceArray_base& apps, void* data) {
    const COptionsBase* opt = (const COptionsBase*)data;

    // Make sure we have something to work with...
    if (!filterFunc || !preFunc || !postFunc || !displayFunc || !dataFunc) {
        LOG_ERR("traverser function is null.");
        quickQuitHandler(1);
    }

    // Prepare the export...
    if (!(*preFunc)(this, data))
        return false;

    // For each appearance...
    for (index = 0; index < apps.size() && !shouldQuit(); index++) {
        app = &apps[index];
        trans = CTransaction();  // reset
        bool passedFilter = !filterFunc || (*filterFunc)(this, data);
        if (passedFilter) {
            if (displayFunc) {
                // We freshen at most 5,000 new transactions to stay responsive
                if (opt->freshenOnly && nProcessed > 5000)
                    break;
                if (!(*dataFunc)(this, data))
                    return ((*postFunc)(this, data)) && false;
                if (!(*displayFunc)(this, data))
                    return ((*postFunc)(this, data)) && false;
            }
        }
    }

    // Cleanup the export...
    if (!(*postFunc)(this, data))
        return false;

    return true;
}

//-----------------------------------------------------------------------
bool rangeFilter(CTraverser* trav, void* data) {
    const COptionsBase* opt = (COptionsBase*)data;
    if (trav->app->blk > opt->scanRange.second || trav->app->blk >= expContext().tsCnt || shouldQuit())
        return false;
    return inRange(blknum_t(trav->app->blk), opt->scanRange.first, opt->scanRange.second);
}

}  // namespace qblocks
