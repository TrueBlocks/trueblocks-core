#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"

//-------------------------------------------------------------------------
class CBlockVisitor
{
public:
    CBlockVisitor(uint64_t fb, uint64_t c) : m_firstBlock(fb), m_cnt(c) { }
    uint64_t firstBlock() const { return m_firstBlock; }
    uint64_t size() const { return m_cnt; }
    void setFirst(uint64_t n) { m_firstBlock = n; }
    void setCount(uint64_t n) { m_cnt = n; }

protected:
    uint64_t m_firstBlock;
    uint64_t m_cnt;

private:
    CBlockVisitor(void) : m_firstBlock(0), m_cnt(0) { }
};

//-----------------------------------------------------------------------------------------------
class CVisitor : public CBlockVisitor {
public:
    uint64_t nBlocks;
    CBinFile miniB;  // mini block file
    biguint_t sizeB;

    uint64_t nTrans;
    CBinFile miniT;  // mini transaction file
    biguint_t sizeT;

    double startTime;
    bool deep;

    CVisitor(uint64_t first, uint64_t cnt) : CBlockVisitor(first, cnt) {
        nBlocks   = nTrans = 0;
        startTime = qbNow();
        sizeB = sizeT = 0;
        deep = false;
    }

    void interumReport(const CMiniBlock& block) const;
    void finalReport(void) const;
    void getLastBlock(blknum_t maxBlocks);
};

//-----------------------------------------------------------------------------------------------
extern bool buildMiniBlock(CBlock& block, void *data);
