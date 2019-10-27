/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "options.h"
#include "reporter.h"

//-----------------------------------------------------------------------------
int main(int argc, const char *argv[]) {
    etherlib_init(quickQuitHandler);

    CTreeRoot::registerClass();
    CTreeNode::registerClass();
    CInfix::registerClass();
    CBranch::registerClass();
    CLeaf::registerClass();

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        if (options.all) {
            options.startBlock = 0;
            options.nBlocks = getLastBlock_client();
        }

        CFilename fn("accts.bin");
        bool exists = fileExists(fn.getFullPath());
        string_q msg = string_q(exists ? "Reading" : "Accumulating") + " accounts between blocks " +
                            uint_2_Str(options.startBlock) + " and " +
                            uint_2_Str(options.startBlock+options.nBlocks) + " (nBlocks: " +
                            uint_2_Str(options.nBlocks) + ")";

        CReporter reporter;
        reporter.tree = new CTreeRoot;
        if (reporter.tree) {
            reporter.startTimer(msg);
            if (exists) {
                CArchive archive(READING_ARCHIVE);
                if (archive.Lock(fn.getFullPath(), modeReadOnly, LOCK_WAIT)) {
                    reporter.tree->Serialize(archive);
                    archive >> reporter.nBlocksVisited;
                    archive >> reporter.nTransVisited;
                    archive.Close();
                    if (isTestMode())
                        ::remove(fn.getFullPath().c_str());
                }
            } else {
                forEveryBlockOnDisc(buildTree, &reporter, options.startBlock, options.nBlocks);
                CArchive archive(WRITING_ARCHIVE);
                if (archive.Lock(fn.getFullPath(), modeWriteCreate, LOCK_WAIT)) {
                    reporter.tree->Serialize(archive);
                    archive << reporter.nBlocksVisited;
                    archive << reporter.nTransVisited;
                    archive.Close();
                }
            }
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
    for (size_t i = 0 ; i < block.transactions.size() ; i++) {
        CTransaction *tr = reinterpret_cast<CTransaction *>(&block.transactions[i]);
        if (string_q(tr->to).empty())
            tr->to = "0x0";
        r->nTransVisited++;
        r->tree->insert(tr->from, uint_2_Str(block.blockNumber));
        r->tree->insert(tr->to, uint_2_Str(block.blockNumber));
    }
    cerr << ts_2_Date(block.timestamp) << " -- " << r->nBlocksVisited << ": " << r->nTransVisited << "\r";
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
        r->maxDepth  = countOf(r->strs, '-');
        r->getNext = true;
        r->maxMatch1 = r->strs;
    }
    if (r->type == T_LEAF) {
        if (r->counter < MAX_CNTS)
            r->counters[r->counter]++;
        if (r->counter > 5)
            r->gtFive++;
        if (r->counter > r->maxCnt) {
            r->maxCnt = r->counter;
            r->maxStr = r->strs;
        }
    }
    r->counter = 0;

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

