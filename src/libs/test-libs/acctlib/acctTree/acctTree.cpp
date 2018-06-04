#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks(tm) - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
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
#include "reporter.h"

//-----------------------------------------------------------------------------
int main(int argc, const char *argv[]) {

    etherlib_init();

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    if (options.commandList.empty())
        options.commandList = "null";
    while (!options.commandList.empty()) {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        if (options.all) {
            options.startBlock = 0;
            options.nBlocks = getLatestBlockFromClient();
        }

        CReporter reporter;
        reporter.tree = new CTreeRoot;
        if (reporter.tree) {
            //-----------------------------------------------
            SFString msg = "Accumulating accounts between blocks " +
                                asStringU(options.startBlock) + " and " +
                                asStringU(options.startBlock+options.nBlocks) + " (nBlocks: " +
                                asStringU(options.nBlocks) + ")";
            reporter.startTimer(msg);
            forEveryBlockOnDisc(buildTree, &reporter, options.startBlock, options.nBlocks);
            reporter.stopTimer();

            //-----------------------------------------------
            reporter.startTimer("Displaying accounts...");
            forEveryAccount(reporter.tree, printTree, &reporter);
            reporter.stopTimer();
        }
        reporter.finalReport();
    }

    return 0;
}

//-----------------------------------------------------------------------
bool buildTree(CBlock& block, void *data) {

    CReporter *r = reinterpret_cast<CReporter*>(data);

    r->nBlocksVisited++;
    for (uint32_t i = 0 ; i < block.transactions.getCount() ; i++) {
        CTransaction *tr = (CTransaction*)&block.transactions[i];  // NOLINT
        if (SFString(tr->to).empty())
            tr->to = "0x0";
        r->nTransVisited++;
        r->tree->insert(tr->from, asStringU(block.blockNumber));
        r->tree->insert(tr->to, asStringU(block.blockNumber));
    }
    cerr << dateFromTimeStamp(block.timestamp) << " -- " << r->nBlocksVisited << ": " << r->nTransVisited << "\r";
    cerr.flush();
    return true;
}

//-----------------------------------------------------------------------------
bool printTree(const CTreeNode *node, void *data) {
    CReporter *r = reinterpret_cast<CReporter*>(data);

    r->nAccts++;

    // This simply accumulates the longest common prefixes
    if (r->getNext) {
        r->maxMatch2 = r->strs;
        r->getNext = false;
    }

    if (r->isMax()) {
        r->maxDepth  = countOf('-', r->strs);
        r->getNext = true;
        r->maxMatch1 = r->strs;
    }
    if (r->type == T_LEAF) {
        if (r->counter < MAX_CNTS)
            r->counters[r->counter]++;
        if (r->counter > 5)
            r->gtFive++;
        if (r->counter > r->maxCnt) {
            r->maxCnt = r->counter;
            r->maxStr = r->strs;
        }
    }
    r->counter = 0;

    // print the report
    r->interumReport();

    return true;
}

//-----------------------------------------------------------------------------
void CReporter::interumReport(void) {
    SFString types[] = {
        cWhite   +     "T_TRTOP " + cOff,
        cRed     +     "T_TRLEAF" + cOff,
        bYellow  + "\n""T_BRANCH" + cOff,
        cGreen   + "\n""T_INFIX " + cOff,
    };

    cout << types[type] << " " << strs.Substitute("+", "").Substitute("-", "").Substitute("|0", "") << "\n";
}

