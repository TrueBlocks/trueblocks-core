#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
extern SFUint32 startBlock;
extern SFUint32 nBlocks;

//-----------------------------------------------------------------------------
class CReporter : public CVisitData {
public:
    blknum_t nBlocksVisited;
    txnum_t nTransVisited;
    blknum_t nAccts;
    blknum_t maxDepth;
    SFString maxMatch1;
    SFString maxMatch2;
    bool getNext;
    double startLoad;
    double endLoad;
    double startPrint;
    double endPrint;
    SFUint32 stopping;
    CAccountTree *tree;

    CReporter(void)
        : nBlocksVisited(0), nTransVisited(0),
          nAccts(0), maxDepth(0), getNext(false),
          startLoad(0), endLoad(0), startPrint(0), endPrint(0), stopping(30), tree(NULL) { }

    ~CReporter(void) { if (tree) delete tree; }

    void startTimer(const SFString& msg) {
        cerr << msg << "\n"; cerr.flush();
        if (startLoad == 0.0) startLoad = qbNow();
        else                  startPrint = qbNow();
    }

    void stopTimer(void) {
        if (endLoad == 0.0) endLoad = qbNow();
        else                endPrint = qbNow();
    }

    void finalReport(void) {
        double durLoad = (endLoad-startLoad);
        double durPrint = (endPrint-startPrint);
        cerr << "nAccts:    " << cGreen << nAccts         << cOff << "\n";
        cerr << "maxDepth:  " << cGreen << maxDepth       << cOff << "\n";
        cerr << "maxMatch1: " << cGreen << maxMatch1      << cOff << "\n";
        cerr << "maxMatch2: " << cGreen << maxMatch2      << cOff << "\n";
        cerr << "nBlocks:   " << cGreen << nBlocksVisited << cOff << "\n";
        cerr << "nTrans:    " << cGreen << nTransVisited  << cOff << "\n";
        cerr << "Data built in " << cGreen << durLoad
              << " seconds. Data displayed in " << cGreen << durPrint << cOff << " seconds.\n";
        cerr.flush();
    }

    void interumReport(const SFString& from, const SFString& to, SFUint32 bn) {
        if (!(nTransVisited % 1301)) {
            cout << nBlocksVisited << ":" << bn << "." << from << "\n";
            cout << nBlocksVisited << ":" << bn << "." << to   << "\n";
            cout.flush();
        }
    }

    void interumReport(void) {
        SFString str = strs;
        nextTokenClear(str, '-');
        cerr << types[type] << "(" << type << ")" << str << "\n";
    }

    bool isMax(void) {
        if (strs.Contains("-0+-0+-0+-0+-0+-0+-0+-0+-0+-0+-0+000000000000000000000"))
            return false;
        return (countOf('-', strs) > maxDepth);
    }
};

//-----------------------------------------------------------------------------
extern bool printTree(const CTreeNode *node, void *data);
extern bool buildTree(CBlock& block, void *data);
