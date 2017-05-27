/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"
#include "reporter.h"
#include "MemoryMapped.h"

#define N_BLOCKS 2000

bool first = true;
double rate = 1.0;
extern void doOne(uint32_t start, uint32_t stop);
extern bool visitMini(CMiniBlock& block, const CMiniTrans *trans, void *data);
extern bool visitBlock(CBlock& block, void *data);
//--------------------------------------------------------------
int main(int argc, const char *argv[]) {
    // Tell the system where the blocks are and which version to use
    setStorageRoot(BLOCK_CACHE);
    etherlib_init("binary");

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    // while (!options.commandList.empty())
    {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        // Load the cache first so it doesn't effect timing
        verbose = true;
        forEveryMiniBlockInMemory(NULL, NULL, 0, 1);
        cerr << "\n";
        cout << cTeal << "Hit enter to continue..." << cOff;
        getchar();
        verbose = true;

        // Parity is very slow, only do 1,000 blocks
        { CReporter reporter("parity", 1000000, N_BLOCKS);
            forEveryBlockFromClient(visitBlock, &reporter);
            reporter.finalReport(); }
        cout << cTeal << "Hit enter to continue..." << cOff;
        cout.flush();
        getchar();

        // Test the various on disc, database versions of the chain
        { CReporter reporter("binary", 1000000, N_BLOCKS);
            forEveryBlockOnDisc(visitBlock, &reporter);
            reporter.finalReport(); }
        cout << cTeal << "Hit enter to continue..." << cOff;
        cout.flush();
        getchar();

        { CReporter reporter("binary", 1000000, N_BLOCKS * 10);
            forEveryBlockOnDisc(visitBlock, &reporter);
            reporter.finalReport(); }
        cout << cTeal << "Hit enter to continue..." << cOff;
        cout.flush();
        getchar();

        { CReporter reporter("nonemp", 1000000, N_BLOCKS * 10);
            forEveryNonEmptyBlockOnDisc(visitBlock, &reporter);
            reporter.finalReport(); }
        cout << cTeal << "Hit enter to continue..." << cOff;
        cout.flush();
        getchar();

        { CReporter reporter("mem", 1000000, N_BLOCKS * 10);
            forEveryFullBlockInMemory(visitBlock, &reporter);
            reporter.finalReport(); }
        cout << cTeal << "Hit enter to continue..." << cOff;
        cout.flush();
        getchar();

        doOne(1000000, N_BLOCKS * 10);
        cout << "\n" << cTeal << "Hit enter to continue..." << cOff;
        cout.flush();
        getchar();

        { CReporter reporter("mem", 1000000, N_BLOCKS * 100);
            forEveryMiniBlockInMemory(visitMini, &reporter);
            reporter.finalReport(); }

    }
    return 0;
}

//-----------------------------------------------------------------------
bool visitBlock(CBlock& block, void *data) {
    CReporter *reporter = reinterpret_cast<CReporter*>(data);

    reporter->nBlocksVisited++;
    reporter->totalGasUsed += canonicalWei(block.gasUsed);
    for (uint32_t tr = 0; tr < block.transactions.getCount(); tr++) {
        reporter->nTransVisited++;
        reporter->totalWei += block.transactions[tr].value;
    }
    reporter->interumReport((uint32_t)block.blockNumber, block.transactions.getCount());
    //  reporter->finalReport();

    return true;
}

//-----------------------------------------------------------------------
bool visitMini(CMiniBlock& block, const CMiniTrans *trans, void *data) {
    CReporter *reporter = reinterpret_cast<CReporter*>(data);

    reporter->nBlocksVisited++;
    for (uint32_t tr = (uint32_t)block.firstTrans; tr < block.firstTrans+block.nTrans; tr++) {
        const CMiniTrans *mini = &trans[tr];
        reporter->nTransVisited++;
        reporter->totalWei += canonicalWei(mini->value);
        reporter->totalGasUsed += canonicalWei(mini->gasUsed);
    }
    reporter->interumReport((uint32_t)block.blockNumber, (uint32_t)block.nTrans);

    return true;
}

//-----------------------------------------------------------------------
void doOne(uint32_t start, uint32_t stop) {
    CReporter reporter("mem", start, stop);
    verbose = true;

    MemoryMapped blockFile(miniBlockCache.c_str(), MemoryMapped::WholeFile, MemoryMapped::SequentialScan);
    MemoryMapped transFile(miniTransCache.c_str(),  MemoryMapped::WholeFile, MemoryMapped::SequentialScan);

    const CMiniBlock *blocks = reinterpret_cast<const CMiniBlock *>(blockFile.getData());
    const CMiniTrans *trans  = reinterpret_cast<const CMiniTrans *>(transFile.getData());

    uint64_t nBlocks = blockFile.size() / sizeof(CMiniBlock);
    bool done = (nBlocks < 1);
    uint32_t b = 0;
    while (!done) {
        if (blocks[b].blockNumber >= start)
        {
            CMiniBlock mb = blocks[b];
            visitMini(mb, trans, &reporter);
        }
        b++;
        //      cerr << b << "\r";cerr.flush();
        if (blocks[b].blockNumber >= (start+stop) || b > nBlocks)
            done = true;
    }
    reporter.finalReport();
}
