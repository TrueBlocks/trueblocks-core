/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

bool logFilter(const CLogEntry& log, const COptions* opt);
//-----------------------------------------------------------------------
bool logs_Display(CTraverser* trav, void* data) {
    COptions* opt = (COptions*)trav->options;

    trav->nProcessed += trav->trans.receipt.logs.size();
    if (opt->freshen)
        return true;

    for (auto log : trav->trans.receipt.logs) {
        if (logFilter(log, opt)) {
            cout << ((isJson() && !opt->firstOut) ? ", " : "");
            cout << log;
            opt->firstOut = false;
        }
    }

    prog_Log(trav, data);
    return !shouldQuit();
}

//-----------------------------------------------------------------------
bool logs_Pre(CTraverser* trav, void* data) {
    COptions* opt = (COptions*)trav->options;
    opt->firstOut = true;

    if (opt->apps.size() > 0 && opt->first_record != 0)
        opt->lastStatement.endBal = getBalanceAt(opt->accountedFor, opt->apps[0].blk - 1);

    start_Log(trav, data);
    return true;
}

//-----------------------------------------------------------------------
bool logFilter(const CLogEntry& log, const COptions* opt) {
    if (!opt->source.empty()) {
        if (!opt->wasEmittedBy(log.address))
            return false;
        return !opt->relevant || opt->isRelevant(log);
    }

    if (opt->emitter) {
        return opt->isEmitter(log.address);
    }

    if (opt->topics.size() && log.topics.size()) {
        for (auto topic : opt->topics) {
            if (topic == log.topics[0])
                return true;
        }
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------
bool COptions::handle_logs(void) {
    CTraverser trav(this, cout, "logs");
    trav.preFunc = logs_Pre;
    trav.displayFunc = logs_Display;

    CTraverserArray traversers;
    traversers.push_back(trav);

    forEveryAppearance(traversers, apps, nullptr);

    return !shouldQuit();
}
