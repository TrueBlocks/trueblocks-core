/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "options.h"
#include "reporter.h"

SFUint32 maxBlocks = 25000;
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

        //verbose = 2;
        if (options.all) {
            options.startBlock = 4000000;
            maxBlocks = getLatestBlockFromClient();
        }

        CReporter reporter;
        reporter.tree = new CTreeRoot;
        if (reporter.tree) {
            //-----------------------------------------------
            reporter.startTimer("Accumulating accounts...");
            forEveryBlockOnDisc(buildTree, &reporter, options.startBlock, getLatestBlockFromClient());
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

    CReporter *r = reinterpret_cast<CReporter*>(data);

    r->nBlocksVisited++;
    for (uint32_t i = 0 ; i < block.transactions.getCount() ; i++) {
        CTransaction *tr = (CTransaction*)&block.transactions[i];  // NOLINT
        if (SFString(tr->to).empty())
            tr->to = "0x0";
        r->nTransVisited++;
        r->tree->insert(tr->from, asString(block.blockNumber));
        r->tree->insert(tr->to, asString(block.blockNumber));
    }
    cerr << r->nBlocksVisited << ":" << block.blockNumber << "\r";
    cerr.flush();
    return (r->nBlocksVisited < maxBlocks);
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

