/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "options.h"
#include "reporter.h"

//-----------------------------------------------------------------------------
int main(int argc, const char *argv[]) {

    // Tell the system where the blocks are and which version to use
    setStorageRoot(BLOCK_CACHE);
    etherlib_init("binary");

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
        if (r->cnt < MAX_CNTS)
            r->counters[r->cnt]++;
        if (r->cnt > 5)
            r->gtFive++;
        if (r->cnt > r->maxCnt) {
            r->maxCnt = r->cnt;
            r->maxStr = r->strs;
        }
    }
    r->cnt = 0;

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

