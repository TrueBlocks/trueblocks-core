#pragma once
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
#define MAX_CNTS 20
//-----------------------------------------------------------------------------
class CReporter : public CVisitData {
public:
    blknum_t nBlocksVisited;
    txnum_t nTransVisited;
    blknum_t nAccts;
    blknum_t maxDepth;
    string_q maxMatch1, maxMatch2, maxStr;
    blknum_t counters[MAX_CNTS], maxCnt, gtFive;
    bool getNext;
    double startLoad;
    double endLoad;
    double startPrint;
    double endPrint;
    uint64_t stopping;
    CTreeRoot *tree;

    CReporter(void)
        : nBlocksVisited(0), nTransVisited(0),
          nAccts(0), maxDepth(0), maxCnt(0), gtFive(0),
          getNext(false), startLoad(0), endLoad(0),
          startPrint(0), endPrint(0), stopping(30), tree(NULL) {
              bzero(counters, sizeof(counters));
          }

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

    string_q asPct(double _part, double _whole) {
        double percent = 1.0;
        if (_whole > 0)
            percent = (_part / static_cast<double>(_whole));
        return double_2_Str(100.*percent, 3);
    }

    void finalReport(void) {


        cout << "nAccts:    " << cGreen << nAccts         << cOff << "\n";
        cout << "maxDepth:  " << cGreen << maxDepth       << cOff << "\n";
        cout << "maxMatch1: " << cGreen << maxMatch1      << cOff << "\n";
        cout << "maxMatch2: " << cGreen << maxMatch2      << cOff << "\n";
        for (uint64_t i = 0 ; i < MAX_CNTS ; i++) {
            cout << padNum3(i) << ":    " << cGreen
                    << counters[i] << " (" << asPct(counters[i], nTransVisited) << "%)" << cOff << "\n";
        }
        cout << "maxCnt:    " << cGreen << maxCnt
                << " (" << asPct(maxCnt, nTransVisited) << "%)" << cOff << "\n";
        cout << "gtFive:    " << cGreen << gtFive
                << " (" << asPct(gtFive, nTransVisited) << "%)" << cOff << "\n";
        cout << "maxStr:    " << cGreen << maxStr         << cOff << "\n";
        cout << "nBlocks:   " << cGreen << nBlocksVisited << cOff << "\n";
        cout << "nTrans:    " << cGreen << nTransVisited  << cOff << "\n";
        cout.flush();
    }

    void interumReport(void);
    bool isMax(void) {
        if (contains(strs, "0x+-0+-0+-0+-0+-0+-0+-0+-0+-0+-0+-0+-0+000000000000"))
            return false;
        return (countOf(strs, '-') > maxDepth);
    }
};

//-----------------------------------------------------------------------------
extern bool printTree(const CTreeNode *node, void *data);
extern bool buildTree(CBlock& block, void *data);
