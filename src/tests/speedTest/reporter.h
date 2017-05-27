#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"

extern double rate;
extern bool first;
//-------------------------------------------------------------------------
class CReporter : public CBlockVisitor {
public:
    SFString  src;
    uint32_t  nBlocksVisited;
    uint32_t  nTransVisited;
    SFUintBN  totalWei;
    SFUintBN  totalGasUsed;
    double    startTime;
    uint32_t  repFreq;

public:
    CReporter(const SFString& mode, uint32_t st, uint32_t n)
    : CBlockVisitor(st, n),
        nBlocksVisited(0), nTransVisited(0),
        totalWei(0), totalGasUsed(0),
        startTime(0.), repFreq(133) { startTimer(mode); }
    ~CReporter(void) {  }

    void interumReport(const SFString& curBlock, uint32_t tThis) const {
        uint32_t bn = toLong32u(curBlock);
        if (curBlock.startsWith("0x"))
            bn = (uint32_t)hex2Long(curBlock);
        interumReport(bn, tThis);
    }
    void interumReport(uint32_t curBlock, uint32_t tThis) const {
        if (src == "parity")
            return;

        if (!(nBlocksVisited % repFreq))
            progressBar(nBlocksVisited, getCount(), qbNow()-startTime);

        //  {
        //      cerr << src << ":\tBlock " << conYellow << "#" << asYellow(nBlocksVisited << "/" << curBlock);
        //      cerr << " ("     << asYellow(padNum3T(tThis) << "/" << asString(nTransVisited)) << " transactions)\r";
        ////    cerr << src << ":\t" << bn << "." << nTransVisited << "." << totalGasUsed << "." << totalWei << "\r";
        //      cerr.flush();
        //  }
    }

    void finalReport(void) const {
        double endTime  = qbNow();
        double duration = (endTime-startTime);
        progressBar(getCount(), getCount(), qbNow()-startTime);

        cerr << "\n" << src.Substitute("mem", (first?"winput":"base")) << "\t"
        << cGreen << padNum8T(nBlocksVisited) << cOff << " blocks (of " << padNum8T(getCount()) << ") / "
        << cGreen << padNum8T(nTransVisited) << cOff << " trans / "
        << cGreen << totalWei << cOff << " wei / "
        << cGreen << totalGasUsed << cOff << " gas / "
        << cRed << duration << cOff << " seconds. "
        << asYellow(nBlocksVisited/(duration)) << " b/s. "
        << asYellow(nTransVisited/(duration))  << " t/s ";

        if (src == "mem")
            first = false;
        SFString speedup;
        if (rate == 1.0) {
            rate = (nBlocksVisited/duration);
        } else {
            double r = (nBlocksVisited/duration);
            double t = (r/rate);
            int rr = (t*100.);
            double d = (rr/100.);
            cerr << asYellow(d) << " X faster than RPC";
        }
        cerr << "\n\n";
        cerr.flush();
    }

    void startTimer(const SFString& source) {
        etherlib_init(source);
        startTime = qbNow();
        nTransVisited = 0;
        nBlocksVisited = 0;
        totalWei  = 0;
        totalGasUsed = 0;
        // *this = CBlockVisitor(0, 0);
        src = source;
    }
};
