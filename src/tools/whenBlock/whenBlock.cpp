/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//---------------------------------------------------------------
extern bool lookupDate(CBlock& block, const SFTime& date);
extern void unloadCache(void);

//---------------------------------------------------------------
int main(int argc, const char *argv[]) {
    setStorageRoot(BLOCK_CACHE);
    etherlib_init("parity");

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    bool vb = !options.commandList.Contains("\n");
    while (!options.commandList.empty()) {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        CBlock block;
        if (options.blockNum != NOPOS) {
            if (verbose) { cout << "finding by block number\n"; }
            queryBlock(block, asStringU(options.blockNum), false);

        } else {
            if (verbose) { cout << "finding by date\n"; }
            bool res = lookupDate(block, options.date);
            if (!res) {
                unloadCache();
                return 0;
            }
        }

        // special case for the zero block
        if (block.blockNumber == 0)
            block.timestamp = 1438269960;

        if (options.alone) {
            if (options.blockNum != NOPOS) {
                cout << dateFromTimeStamp(block.timestamp) << "\n";
            } else {
                cout << block.blockNumber << "\n";
            }

        } else {
            cout << (vb?"\n\t":"") << "block " << asYellow("#" << block.blockNumber)
                << " : " << asYellow(block.timestamp) << " : "
                << asYellow(dateFromTimeStamp(block.timestamp)) << (vb?"\n":"") << "\n";
        }
    }

    unloadCache();
    return 0;
}

//---------------------------------------------------------------
static CMiniBlock *blocks = NULL;
static SFUint32 nBlocks = 0;
static SFUint32 below = ULONG_MAX;
static SFUint32 above = 0;
#define tD(a) dateFromTimeStamp(a)

//---------------------------------------------------------------
int findFunc(const void *v1, const void *v2) {
    const CMiniBlock *m1 = (const CMiniBlock *)v1;
    const CMiniBlock *m2 = (const CMiniBlock *)v2;
    below = (m1->timestamp > m2->timestamp ? m2->blockNumber : below);
    above = (m1->timestamp < m2->timestamp ? m2->blockNumber : above);
    return static_cast<int>(m1->timestamp - m2->timestamp);
}

//---------------------------------------------------------------
class CBlockFinder {
public:
    timestamp_t ts;
    SFUint32 found;
    explicit CBlockFinder(timestamp_t t) : ts(t), found(0) { }
};

//---------------------------------------------------------------
bool lookCloser(CBlock& block, void *data) {
//    cout << "Checking block: " << block.blockNumber << " (" << dateFromTimeStamp(block.timestamp) << ")";
    CBlockFinder *bf = reinterpret_cast<CBlockFinder*>(data);
    if (block.timestamp <= bf->ts) {
//        cout << "...saved";
        bf->found = block.blockNumber;
//        cout << "\n";
        return true;
    }
//    cout << "\n";
    return false;
}

//---------------------------------------------------------------
bool lookupDate(CBlock& block, const SFTime& date) {
    if (!blocks) {
        nBlocks = fileSize(miniBlockCache) / sizeof(CMiniBlock);
        blocks = new CMiniBlock[nBlocks];
        if (!blocks)
            return usage("Could not allocate memory for the blocks (size needed: " + asStringU(nBlocks) + ").\n");
        bzero(blocks, sizeof(CMiniBlock)*(nBlocks));
        if (verbose)
            cerr << "Allocated room for " << nBlocks << " miniBlocks.\n";

        // Next, we try to open the mini-block database (caller will cleanup)
        FILE *fpBlocks = fopen(miniBlockCache.c_str(), binaryReadOnly);
        if (!fpBlocks)
            return usage("Could not open the mini-block database: " + miniBlockCache + ".\n");

        // Read the entire mini-block database into memory in one chunk
        size_t nRead = fread(blocks, sizeof(CMiniBlock), nBlocks, fpBlocks);
        if (nRead != nBlocks)
            return usage("Error encountered reading mini-blocks database.\n Quitting...");
        if (verbose)
            cerr << "Read " << nRead << " miniBlocks into memory.\n";
    }

    CMiniBlock mini;
    mini.timestamp = toTimeStamp(date);
    CMiniBlock *found = reinterpret_cast<CMiniBlock*>(bsearch(&mini, blocks, nBlocks, sizeof(CMiniBlock), findFunc));
    if (found) {
        queryBlock(block, asStringU(found->blockNumber), false);
        return true;
    }

    //  cout << mini.timestamp << " is somewhere between " << below << " and " << above << "\n";
    CBlockFinder finder(mini.timestamp);
    forEveryBlockOnDisc(lookCloser, &finder, below, above-below);
    queryBlock(block, asStringU(finder.found), false);
    return true;
}

//---------------------------------------------------------------
void unloadCache(void) {
    if (blocks) {
        delete [] blocks;
        blocks = NULL;
    }
}
