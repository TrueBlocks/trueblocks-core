/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "tree.h"
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

    //while (!options.commandList.empty())
    {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        CReporter reporter;
        reporter.tree = new CAccountTree;
        if (reporter.tree) {
            //-----------------------------------------------
            reporter.startTimer("Accumulating accounts...");
            forEveryBlockOnDisc(buildTree, &reporter, 2500000, getClientLatestBlk());
            reporter.stopTimer();

            //-----------------------------------------------
//            cerr << "\nHit enter to continue...\n";
//            if (!isTesting)
//                getchar();

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

    static int x = 0;
    CReporter *r = reinterpret_cast<CReporter*>(data);

    r->nBlocksVisited++;
    for (uint32_t i = 0 ; i < block.transactions.getCount() ; i++) {
        CTransaction *tr = (CTransaction*)&block.transactions[i];  // NOLINT
        if (SFString(tr->to).empty())
            tr->to = "0x0";
        r->nTransVisited++;
        r->tree->insert(tr->from, asString(block.blockNumber));
        r->tree->insert(tr->to, asString(block.blockNumber));
        r->interumReport(tr->from, tr->to, block.blockNumber);
        x++;
    }
    return (x < 50000);
}

//-----------------------------------------------------------------------------
bool printTree(const CTreeNode *node, void *data) {
    CReporter *r = reinterpret_cast<CReporter*>(data);

    r->nAccts++;

    // Stop increasing less frequenty while printing so we can see what's going on
//    if (!isTesting && !(r->nAccts % r->stopping)) {
//        char ch = getchar();
//        if (ch == 'q')
//            exit(0);
//        r->stopping*=2;
//    }

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

    // print the report
    r->interumReport();

    return true;
}
