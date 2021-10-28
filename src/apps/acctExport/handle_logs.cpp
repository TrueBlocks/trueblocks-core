/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "options.h"

bool filterLog(const CLogEntry& log, const COptions* opt);
//-----------------------------------------------------------------------
bool logs_Display(CTraverser* trav, void* data) {
    COptions* opt = (COptions*)data;

    for (auto log : trav->trans.receipt.logs) {
        if (filterLog(log, opt)) {
            log.timestamp = trav->trans.timestamp;
            cout << ((isJson() && !opt->firstOut) ? ", " : "");
            cout << log;
            opt->firstOut = false;
        }
    }

    prog_Log(trav, data);
    return !shouldQuit();
}

//-----------------------------------------------------------------------
size_t logs_Count(CTraverser* trav, void* data) {
    return trav->trans.receipt.logs.size();
}

//-----------------------------------------------------------------------
bool filterLog(const CLogEntry& log, const COptions* opt) {
    if (!opt->logFilter.addresses.empty()) {
        if (!opt->wasEmittedBy(log.address))
            return false;
        return !opt->relevant || opt->isRelevant(log);
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
