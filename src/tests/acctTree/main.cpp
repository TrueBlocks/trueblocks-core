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
            verbose = !isTesting;
            forEveryMiniBlockInMemory(buildTree, &reporter, 0, getClientLatestBlk());
            verbose = false;
            reporter.stopTimer();

            //-----------------------------------------------
            cerr << "\nHit enter to continue...\n";
            if (!isTesting)
                getchar();

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
bool buildTree(CMiniBlock& block, const CMiniTrans *trans, void *data) {
    verbose = false;
    CReporter *r = reinterpret_cast<CReporter*>(data);

    r->nBlocksVisited++;
    for (SFUint32 i = block.firstTrans ; i < block.firstTrans + block.nTrans ; i++) {
        CMiniTrans *tr = (CMiniTrans*)&trans[i];  // NOLINT
        if (SFString(tr->to).empty()) {
            tr->to[0] = '0';
            tr->to[1] = 'x';
            tr->to[2] = '0';
            tr->to[3] = '\0';
        }
        r->nTransVisited++;
        r->tree->insert(tr->from, asString(block.blockNumber));
        r->tree->insert(tr->to, asString(block.blockNumber));
        r->interumReport(tr->from, tr->to, block.blockNumber);
    }
    return true;
}

//-----------------------------------------------------------------------------
bool printTree(const acctTree_Node *node, void *data) {
    CReporter *r = reinterpret_cast<CReporter*>(data);

    r->nAccts++;

    // Stop increasing less frequenty while printing so we can see what's going on
    if (!isTesting && !(r->nAccts % r->stopping)) {
        char ch = getchar();
        if (ch == 'q')
            exit(0);
        r->stopping*=2;
    }

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
