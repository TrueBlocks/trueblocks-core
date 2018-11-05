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
