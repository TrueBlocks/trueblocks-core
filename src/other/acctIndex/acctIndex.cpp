/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "options.h"
#include "reporter.h"

uint64_t maxBlocks = 0; //getLatestBlockFromClient();
//-----------------------------------------------------------------------------
int main(int argc, const char *argv[]) {

    etherlib_init();

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    if (options.commandList.empty())
        options.commandList = "null";
//    while (!options.commandList.empty())
    {
        string_q command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        blknum_t start = 0;
        maxBlocks      = getLatestBlockFromClient();
        CReporter reporter;
        reporter.tree = new CTreeRoot;

        bool isWrite = false;
        if (reporter.tree) {

            while (!options.mode.empty()) {
                string_q mode = nextTokenClear(options.mode,'|');
                if (mode == "list") {
                    cout << "Listing\n";
                    CArchive treeCache(READING_ARCHIVE);
                    if (treeCache.Lock(accountIndex, binaryReadOnly, LOCK_NOWAIT)) {
                        reporter.tree->Serialize(treeCache);
                        treeCache.Release();
                    }
                } else {
                    //-----------------------------------------------
                    cout << "Building\n";
                    reporter.startTimer("Accumulating accounts...");
                    forEveryBlockOnDisc(buildTree, &reporter, start, getLatestBlockFromClient());
                    reporter.stopTimer();
                    isWrite = true;
                }
                
                //-----------------------------------------------
                reporter.startTimer("Displaying accounts...");
                forEveryAccount(reporter.tree, printTree, &reporter);
                reporter.stopTimer();
            }
            reporter.finalReport();
        }

        if (isWrite) {
            CArchive treeCache(WRITING_ARCHIVE);
            if (treeCache.Lock(accountIndex, binaryWriteCreate, LOCK_CREATE)) {
                reporter.tree->Serialize(treeCache);
                treeCache.Release();
            }
            delete reporter.tree;
            reporter.tree = NULL;
        }
    }

    return 0;
}

//-----------------------------------------------------------------------
bool buildTree(CBlock& block, void *data) {

    CReporter *r = reinterpret_cast<CReporter*>(data);

    r->nBlocksVisited++;
    for (uint32_t i = 0 ; i < block.transactions.size() ; i++) {
        CTransaction *tr = (CTransaction*)&block.transactions[i];  // NOLINT
        if (string_q(tr->to).empty())
            tr->to = "0x0";
        r->nTransVisited++;
        r->tree->insert(tr->from, uint_2_Str(block.blockNumber));
        r->tree->insert(tr->to,   uint_2_Str(block.blockNumber));
        cerr << r->nBlocksVisited << ":" << block.blockNumber << ":" << tr->transactionIndex << "      \n";
        cerr.flush();
    }
    cerr << r->nBlocksVisited << ":" << block.blockNumber << "\t\r";
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
        r->maxDepth  = countOf(r->strs, '-');
        r->getNext = true;
        r->maxMatch1 = r->strs;
    }

    // print the report
	r->interumReport();

    return true;
}

//-----------------------------------------------------------------------------
void CReporter::interumReport(void) {
    string_q types[] = {
        cWhite   +     "T_TRTOP " + cOff,
        cRed     +     "T_TRLEAF" + cOff,
        bYellow  + "\n""T_BRANCH" + cOff,
        cGreen   + "\n""T_INFIX " + cOff,
    };
    cout << types[type] << " " << substitute(substitute(substitute(strs, "+", ""), "-", ""), "|0", "") << "\n";
}

