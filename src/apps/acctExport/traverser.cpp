/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
bool forEveryAppearance(const CTraverserArray& traversers, const CAppearanceArray_base& apps, void* data) {
    // If we have nothing to do, return success
    if (!apps.size() || !traversers.size())
        return true;

    // for each traverser...
    for (auto trav : traversers) {
        if (shouldQuit())
            break;

        // Make sure we have something to work with...
        trav.preFunc = trav.preFunc ? trav.preFunc : noopFunc;
        trav.postFunc = trav.postFunc ? trav.postFunc : noopFunc;

        // Prepare the export...
        if (!(*trav.preFunc)(&trav, data))
            return false;

        // For each appearance...
        for (trav.index = 0; trav.index < apps.size() && !shouldQuit(); trav.index++) {
            trav.app = &apps[trav.index];
            bool passedFilter = !trav.filterFunc || (*trav.filterFunc)(&trav, data);
            if (passedFilter) {
                if (trav.displayFunc) {
                    COptions* opt = (COptions*)trav.options;  // makes code clearer
                    // We freshen at most 5,000 new transactions to stay responsive
                    if (opt->freshen && opt->nProcessed > 5000)
                        break;

                    bool ret = (*trav.displayFunc)(&trav, data);
                    if (!ret)
                        return (*trav.postFunc)(&trav, data) && false;
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
bool rangeFilter(const CTraverser* trav, void* data) {
    if (trav->app->blk > trav->options->scanRange.second || trav->app->blk >= expContext().tsCnt || shouldQuit())
        return false;

    return inRange(blknum_t(trav->app->blk), trav->options->scanRange.first, trav->options->scanRange.second);
}

//-----------------------------------------------------------------------
bool defPostFunc(const CTraverser* trav, void* data) {
    COptions* opt = (COptions*)trav->options;

    if (trav->lastExpBlock != NOPOS)
        for (auto monitor : opt->allMonitors)
            monitor.writeLastExport(trav->lastExpBlock);

    opt->reportNeighbors();

    end_Log(trav, data);
    return true;
}

#define REPORT_FREQ 5
//-----------------------------------------------------------------------
void prog_Log(const CTraverser* trav, void* data, TraverserLog mode) {
    if (!trav->logging)
        return;
    ASSERT(mode == TR_PROGRESS_CACHE || mode == TR_PROGRESS_NODE);

    const COptions* opt = trav->options;
    if (opt->nProcessed % REPORT_FREQ)
        return;

    // TODO: Use inCache to distinguish between Reading and Fetching
    blknum_t prog = opt->first_record + opt->nProcessed;
    blknum_t goal = opt->nTransactions;
    ostringstream post;
    post << " " << trav->op << " (max " << goal << ") for address " << opt->hackAppAddr;
    LOG_PROGRESS((mode == TR_PROGRESS_CACHE ? "Reading" : "Extracting"), prog, goal, post.str() + "\r");
    return;
}

//-----------------------------------------------------------------------
void end_Log(const CTraverser* trav, void* data) {
    if (!trav->logging)
        return;

    const COptions* opt = trav->options;
    blknum_t prog = opt->first_record + opt->nProcessed;
    blknum_t goal = opt->nTransactions;
    if (prog == goal) {
        string_q msg = opt->freshen ? "Finished updating" : "Finished reporting on";
        string_q endMsg = " " + trav->op + " for address " + opt->hackAppAddr;
        LOG_PROGRESS(msg, prog, goal, endMsg);
    }
    return;
}

//-----------------------------------------------------------------------
void start_Log(const CTraverser* trav, void* data) {
    if (!trav->logging)
        return;
    return;
}
