/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//---------------------------------------------------------------
extern bool lookupDate(const COptions *options, CBlock& block, const time_q& date);
extern void unloadCache(void);

//---------------------------------------------------------------
int main(int argc, const char *argv[]) {

    etherlib_init("binary", quickQuitHandler);

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        for (auto value : options.requests) {
            string_q mode = nextTokenClear(value, ':');
            string_q special;
            if (mode == "special") {
                mode = "block";
                special = nextTokenClear(value, '|');
                if (str_2_Uint(value) > getLatestBlockFromClient()) {
                    cerr << "The block number you requested (";
                    cerr << cTeal << special << ": " << value << cOff;
                    cerr << ") is after the latest block (";
                    cerr << cTeal << (isTestMode() ? "TESTING" : uint_2_Str(getLatestBlockFromClient())) << cOff;
                    cerr << "). Quitting...\n";
                    return 0;
                }
            }

            CBlock block;
            if (mode == "block") {
                queryBlock(block, value, false, false);

            } else if (mode == "date") {
                if (!fileExists(miniBlockCache)) {
                    cerr << "Looking up blocks by date is not supported without a miniBlock ";
                    cerr << "database, which is an advanced feature.\n";

                } else {
                    time_q date = ts_2_Date((timestamp_t)str_2_Uint(value));
                    bool found = lookupDate(&options, block, date);
                    if (!found) {
                        unloadCache();
                        return 0;
                    }
                }
            }

            // special case for the zero block
            if (block.blockNumber == 0)
                block.timestamp = 1438269960;

            string_q def = (options.alone ?
                                    "[{BLOCKNUMBER}\\t][{DATE}]\\n" :
                                    "block #[{BLOCKNUMBER}][ : {TIMESTAMP}][ : {DATE}]\\n");
            string_q fmt = getGlobalConfig()->getDisplayStr(options.alone, def);
            // we never want to print JSON
            if (fmt.empty()) fmt = substitute(def, "\\n" , "\n");
            if (verbose && !special.empty()) {
                string_q sp = "(" + special + ")";
                replace(fmt, "{BLOCKNUMBER}", "{BLOCKNUMBER} " + sp);
            }
            cout << block.Format(fmt);
        }
    }

    unloadCache();
    return 0;
}

//---------------------------------------------------------------
//TODO(tjayrush): global data
// This global data is fine since this program is not threaded.
static blknum_t    g_lower = ULONG_MAX;
static blknum_t    g_higher = 0;
static CMiniBlock *g_miniBlocks = NULL;
static uint64_t    g_nBlocks = 0;
//---------------------------------------------------------------
int findFunc(const void *v1, const void *v2) {
    const CMiniBlock *m1 = (const CMiniBlock *)v1;
    const CMiniBlock *m2 = (const CMiniBlock *)v2;
    g_lower  = (m1->timestamp > m2->timestamp ? m2->blockNumber : g_lower);
    g_higher = (m1->timestamp < m2->timestamp ? m2->blockNumber : g_higher);
//cout << " b1: " << m1->blockNumber;
//cout << " (" << m1->timestamp << " - " << ts_2_Date(m1->timestamp);
//cout << ") b2: " << m2->blockNumber;
//cout << " (" << m2->timestamp << " - " << ts_2_Date(m2->timestamp);
//cout << ") d: " << (m1->timestamp - m2->timestamp);
//cout << " l: " << g_lower;
//cout << " h: " << g_higher << "\n";
//cout.flush();
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
bool lookupDate(const COptions *options, CBlock& block, const time_q& date) {
    if (!g_miniBlocks) {
        g_nBlocks = fileSize(miniBlockCache) / sizeof(CMiniBlock);
        g_miniBlocks = new CMiniBlock[g_nBlocks];
        if (!g_miniBlocks)
            return options->usage("Could not allocate memory for the blocks (size needed: " + uint_2_Str(g_nBlocks) + ").\n");
        bzero(g_miniBlocks, sizeof(CMiniBlock)*(g_nBlocks));
        if (verbose)
            cerr << "Allocated room for " << g_nBlocks << " miniBlocks.\n";

        // Next, we try to open the mini-block database (caller will cleanup)
        FILE *fpBlocks = fopen(miniBlockCache.c_str(), binaryReadOnly);
        if (!fpBlocks)
            return options->usage("Could not open the mini-block database: " + miniBlockCache + ".\n");

        // Read the entire mini-block database into memory in one chunk
        size_t nRead = fread(g_miniBlocks, sizeof(CMiniBlock), g_nBlocks, fpBlocks);
        if (nRead != g_nBlocks)
            return options->usage("Error encountered reading mini-blocks database.\n Quitting...");
        if (verbose)
            cerr << "Read " << nRead << " miniBlocks into memory.\n";
    }

    CMiniBlock mini;
    mini.timestamp = date_2_Ts(date);
    CMiniBlock *found = reinterpret_cast<CMiniBlock*>(bsearch(&mini, g_miniBlocks, g_nBlocks, sizeof(CMiniBlock), findFunc));
    if (found) {
        queryBlock(block, uint_2_Str(found->blockNumber), false, false);
        return true;
    }

//cout << mini.timestamp << " is somewhere between " << g_lower << " and " << g_higher << "\n";
    CBlockFinder finder(mini.timestamp);
    forEveryBlockOnDisc(lookCloser, &finder, g_lower, g_higher-g_lower);
    queryBlock(block, uint_2_Str(finder.found), false, false);
    return true;
}

//---------------------------------------------------------------
void unloadCache(void) {
    if (g_miniBlocks) {
        delete [] g_miniBlocks;
        g_miniBlocks = NULL;
    }
}
