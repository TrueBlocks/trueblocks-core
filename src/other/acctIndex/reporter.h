#pragma once
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
extern uint64_t startBlock;
extern uint64_t nBlocks;

//-----------------------------------------------------------------------------
class CReporter : public CVisitData {
public:
    blknum_t nBlocksVisited;
    txnum_t nTransVisited;
    blknum_t nAccts;
    blknum_t maxDepth;
    string_q maxMatch1;
    string_q maxMatch2;
    bool getNext;
    double startLoad;
    double endLoad;
    double startPrint;
    double endPrint;
    uint64_t stopping;
    CTreeRoot *tree;

    CReporter(void)
        : nBlocksVisited(0), nTransVisited(0),
          nAccts(0), maxDepth(0), getNext(false),
          startLoad(0), endLoad(0), startPrint(0), endPrint(0), stopping(30), tree(NULL) { }

    ~CReporter(void) { if (tree) delete tree; }

    void startTimer(const string_q& msg) {
        cerr << msg << "\n"; cerr.flush();
        if (startLoad == 0.0) startLoad = qbNow();
        else                  startPrint = qbNow();
    }

    void stopTimer(void) {
        if (endLoad == 0.0) endLoad = qbNow();
        else                endPrint = qbNow();
    }

    void finalReport(void) {
        cout << "nAccts:    " << cGreen << nAccts         << cOff << "\n";
        cout << "maxDepth:  " << cGreen << maxDepth       << cOff << "\n";
        cout << "maxMatch1: " << cGreen << maxMatch1      << cOff << "\n";
        cout << "maxMatch2: " << cGreen << maxMatch2      << cOff << "\n";
//        cout << "nBlocks:   " << cGreen << nBlocksVisited << cOff << "\n";
//        cout << "nTrans:    " << cGreen << nTransVisited  << cOff << "\n";
        cout.flush();
    }

    void interumReport(void);
    bool isMax(void) {
        if (contains(strs, "-0+-0+-0+-0+-0+-0+-0+-0+-0+-0+-0+000000000000000000000"))
            return false;
        return (countOf(strs, '-') > maxDepth);
    }
};

//-----------------------------------------------------------------------------
extern bool printTree(const CTreeNode *node, void *data);
extern bool buildTree(CBlock& block, void *data);
