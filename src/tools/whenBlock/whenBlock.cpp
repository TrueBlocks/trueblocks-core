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
    etherlib_init("parity");

    setNoTracing(true); // we never need to trace in this app

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    while (!options.commandList.empty()) {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        for (uint32_t i = 0 ; i < options.requests.getCount() ; i++ ) {

            SFString value = options.requests[i];
            SFString mode = nextTokenClear(value, ':');

            SFString special;
            if (mode == "special") {
                mode = "block";
                special = nextTokenClear(value,'|');
                if (toUnsigned(value) > getLatestBlockFromClient()) {
                    cout << "The block number you requested (";
                    cout << cTeal << special << ": " << value << cOff;
                    cout << ") is after the latest block (";
                    cout << cTeal << (isTestMode() ? "TESTING" : asStringU(getLatestBlockFromClient())) << cOff;
                    cout << "). Quitting...\n";
                    return 0;
                }
            }

            CBlock block;
            if (mode == "block") {
                queryBlock(block, value, false, false);

            } else if (mode == "date") {
                if (!fileExists(miniBlockCache)) {
                    cout << "Looking up blocks by date is not supported without a miniBlock ";
                    cout << "database, which is an advanced feature.\n";

                } else {
                    SFTime date = dateFromTimeStamp((timestamp_t)toUnsigned(value));
                    bool found = lookupDate(block, date);
                    if (!found) {
                        unloadCache();
                        return 0;
                    }
                }
            }

            // special case for the zero block
            if (block.blockNumber == 0)
                block.timestamp = 1438269960;

            SFString fmt = getGlobalConfig()->getConfigStr("display", "format", "<not_set>");
            if (fmt == "<not_set>")
                fmt = "block #[{BLOCKNUMBER}][ : {TIMESTAMP}][ : {DATE}]";
            if (options.alone) {
                fmt = getGlobalConfig()->getConfigStr("display", "terse", "[{BLOCKNUMBER}\t][{DATE}]");

            } else  {
                fmt.ReplaceAll("{", cTeal+"{");
                fmt.ReplaceAll("}", "}"+cOff);
            }
            if (verbose && !special.empty()) {
                SFString sp = "(" + special + ")";
                fmt.Replace("{BLOCKNUMBER}", "{BLOCKNUMBER} " + sp);
            }
            cout << block.Format(cleanFmt(fmt)) << "\n";
        }
    }

    unloadCache();
    return 0;
}

//---------------------------------------------------------------
static CMiniBlock *blocks = NULL;
static uint64_t nBlocks = 0;
static uint64_t below = ULONG_MAX;
static uint64_t above = 0;
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
    uint64_t found;
    explicit CBlockFinder(timestamp_t t) : ts(t), found(0) { }
};

//---------------------------------------------------------------
bool lookCloser(CBlock& block, void *data) {

    CBlockFinder *bf = reinterpret_cast<CBlockFinder*>(data);
    if (block.timestamp <= bf->ts) {
        bf->found = block.blockNumber;
        return true;
    }
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
    mini.timestamp = toTimestamp(date);
    CMiniBlock *found = reinterpret_cast<CMiniBlock*>(bsearch(&mini, blocks, nBlocks, sizeof(CMiniBlock), findFunc));
    if (found) {
        queryBlock(block, asStringU(found->blockNumber), false, false);
        return true;
    }

    //  cout << mini.timestamp << " is somewhere between " << below << " and " << above << "\n";
    CBlockFinder finder(mini.timestamp);
    forEveryBlockOnDisc(lookCloser, &finder, below, above-below);
    queryBlock(block, asStringU(finder.found), false, false);
    return true;
}

//---------------------------------------------------------------
void unloadCache(void) {
    if (blocks) {
        delete [] blocks;
        blocks = NULL;
    }
}
