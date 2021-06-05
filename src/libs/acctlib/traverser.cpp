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
        if (!trav.traverse(apps, data))
            return false;
    }

    return true;
};

//-----------------------------------------------------------------------
bool CTraverser::traverse(const CAppearanceArray_base& apps, void* data) {
    const COptionsBase* opt = (const COptionsBase*)data;

    // Prepare the export...
    if (preFunc && !(*preFunc)(this, data))
        return false;

    // For each appearance...
    for (index = 0; index < apps.size() && !shouldQuit(); index++) {
        app = &apps[index];
        trans = CTransaction();  // reset
        bool passedFilter = !filterFunc || (*filterFunc)(this, data);
        if (passedFilter) {
            nProcessed += (counterFunc ? (*counterFunc)(this, data) : 1);
            if (!opt->freshenOnly) {
                if (dataFunc && !(*dataFunc)(this, data))
                    return (!postFunc || (*postFunc)(this, data)) && false;
                if (displayFunc && !(*displayFunc)(this, data))
                    return (!postFunc || (*postFunc)(this, data)) && false;
            }
        }
    }

    // Cleanup the export...
    if (postFunc && !(*postFunc)(this, data))
        return false;

    return true;
}

//-----------------------------------------------------------------------
bool filterFunc(CTraverser* trav, void* data) {
    return inRange(blknum_t(trav->app->blk), trav->travRange.first, trav->travRange.second);
}

}  // namespace qblocks
