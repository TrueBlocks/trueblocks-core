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
    etherlib_init(quickQuitHandler);

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
                time_q date = ts_2_Date((timestamp_t)str_2_Uint(value));
                bool found = lookupDate(&options, block, date);
                if (!found)
                    return 0;
            }

            // special case for the zero block
            if (block.blockNumber == 0)
                block.timestamp = 1438269960;

            string_q def = (options.asData ?
                                    "[{BLOCKNUMBER}]\\t[{TIMESTAMP}]\\t[{DATE}]\\n" :
                                    "block #[{BLOCKNUMBER}][ : {TIMESTAMP}][ : {DATE}]\\n");
            string_q fmt = getGlobalConfig("whenBlock")->getDisplayStr(options.asData, def);
            // we never want to print JSON
            if (fmt.empty()) fmt = substitute(def, "\\n" , "\n");
            if (verbose && !special.empty()) {
                string_q sp = "(" + special + ")";
                replace(fmt, "{BLOCKNUMBER}", "{BLOCKNUMBER} " + sp);
            }
            cout << block.Format(fmt);
        }
    }
    return 0;
}

//---------------------------------------------------------------
//TODO(tjayrush): global data
// This global data is fine since this program is not threaded.
static blknum_t         g_lower = ULONG_MAX;
static blknum_t         g_higher = 0;
static CBlockIndexItem *g_dataPtr = NULL;
static uint64_t         g_nBlocks = 0;
//---------------------------------------------------------------
int findFunc(const void *v1, const void *v2) {
    const CBlockIndexItem *m1 = (const CBlockIndexItem *)v1;
    const CBlockIndexItem *m2 = (const CBlockIndexItem *)v2;
    g_lower  = (m1->ts > m2->ts ? m2->bn : g_lower);
    g_higher = (m1->ts < m2->ts ? m2->bn : g_higher);
    return static_cast<int>(m1->ts - m2->ts);
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
    if (!g_dataPtr) {
        g_nBlocks = fileSize(fullBlockIndex_new) / sizeof(CBlockIndexItem);
        g_dataPtr = new CBlockIndexItem[g_nBlocks];  // this allocation gets cleaned up by the options destructor
        if (!g_dataPtr)
            return options->usage("Could not allocate memory for the blocks (size needed: " + uint_2_Str(g_nBlocks) + ").\n");
        bzero(g_dataPtr, sizeof(CBlockIndexItem)*(g_nBlocks));
        if (verbose)
            cerr << "Allocated room for " << g_nBlocks << " index items.\n";

        // Next, we try to open the fullBlocks index database (caller will cleanup)
        FILE *fpBlocks = fopen(fullBlockIndex_new.c_str(), modeReadOnly);
        if (!fpBlocks)
            return options->usage("Could not open the fullBlocks index database: " + fullBlockIndex_new + ".\n");
        // Read the entire fullBlocks index database into memory in one chunk
        size_t nRead = fread(g_dataPtr, sizeof(CBlockIndexItem), g_nBlocks, fpBlocks);
        if (nRead != g_nBlocks)
            return options->usage("Error encountered reading fullBlocks index database. Quitting...");
        if (verbose)
            cerr << "Read " << nRead << " fullBlocks index into memory.\n";
    }

    CBlockIndexItem search;
    search.ts = (uint32_t)date_2_Ts(date);
    CBlockIndexItem *found = reinterpret_cast<CBlockIndexItem*>(bsearch(&search, g_dataPtr, g_nBlocks, sizeof(CBlockIndexItem), findFunc));
    if (found) {
        queryBlock(block, uint_2_Str(found->bn), false, false);
        return true;
    }
    //cout << search.timestamp << " is somewhere between " << g_lower << " and " << g_higher << "\n";
    CBlockFinder finder(search.ts);
    forEveryBlockOnDisc(lookCloser, &finder, g_lower, g_higher-g_lower);
    queryBlock(block, uint_2_Str(finder.found), false, false);
    return true;
}

//---------------------------------------------------------------
void unloadCache(void) {
    if (g_dataPtr) {
        delete [] g_dataPtr;
        g_dataPtr = NULL;
    }
}
