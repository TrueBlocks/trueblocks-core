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
        trav.logFunc = trav.logFunc ? trav.logFunc : noopLogFunc;

        // Prepare the export...
        if (!(*trav.preFunc)(&trav, data))
            return false;

        // For each appearance...
        for (size_t i = 0; i < apps.size() && !shouldQuit(); i++) {
            trav.app = &apps[i];
            bool passedFilter = !trav.filterFunc || (*trav.filterFunc)(&trav, data);
            if (passedFilter) {
                if (trav.displayFunc) {
                    COptions* opt = (COptions*)trav.options;  // makes code clearer
                    opt->nProcessed++;
                    // When freshening, we only freshen 5,000 records to stay responsive
                    if (opt->freshen && opt->nProcessed > 5000)
                        break;

                    bool ret = (*trav.displayFunc)(&trav, data);
                    opt->firstOut = false;
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
void end_Log(const CTraverser* trav, void* data, TraverserLog mode) {
    if (isTestMode())
        return;
    if (mode == TR_END) {
        const COptions* opt = trav->options;
        blknum_t prog = opt->first_record + opt->nProcessed;
        blknum_t goal = min(prog, opt->nTransactions);
        if (prog == goal) {
            string_q msg = opt->freshen ? "Finished updating" : "Finished reporting on";
            string_q endMsg = " " + trav->op + " for address " + opt->hackAppAddr;
            LOG_PROGRESS(msg, prog, goal, endMsg);
        }
    }
}

//-----------------------------------------------------------------------
void prog_Log(const CTraverser* trav, void* data, TraverserLog mode) {
    if (isTestMode())
        return;
    if (mode == TR_PROGRESS) {
        const COptions* opt = trav->options;
        // Use inCache to distinguish between Reading and Fetching
        // ostringstream os;
        // os << "Exported " << opt->nProcessed << " ";
        // os << plural(opt->className) << " from ";
        // os << opt->nProcessing << " txs (blocks: ";
        // os << opt->firstBlock << "-[" << cGreen;
        // os << trav->app->blk << cOff << "]-";
        // os << opt->lastBlock << " ";
        // os << "for address " << opt->hackAppAddr;
        // LOG_INFO(os.str() + "\r");
        blknum_t prog = opt->first_record + opt->nProcessed;
        blknum_t goal = min(prog, opt->nTransactions);
        ostringstream post;
        post << " " << trav->op << " (max " << goal << ") for address " << opt->hackAppAddr;
        LOG_PROGRESS("Extracting", prog, opt->nTransactions, post.str() + "\r");
    }
}
