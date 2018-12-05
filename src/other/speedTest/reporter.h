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

//-------------------------------------------------------------------------
class CSpeedTest : public CBlockVisitor {
public:
    string_q  src;
    uint64_t  nBlocksVisited;
    uint64_t  nTransVisited;
    biguint_t  totalWei;
    biguint_t  totalGasUsed;
    double    startTime;
    double    duration;

public:
    CSpeedTest(const string_q& s, blknum_t st, blknum_t n) : CBlockVisitor(st, n),
        nBlocksVisited(0), nTransVisited(0), totalWei(0), totalGasUsed(0),
        startTime(qbNow()), duration(0.0) {
            etherlib_init(s);
            src = s;
            progressBar(0,0,0.,"");
        }
    void interumReport(blknum_t curBlock, blknum_t tThis);
    void finalReport(const string_q& msg="Hit enter to continue...") const;
};

// Syntactic Sugar
//-------------------------------------------------------------------------
inline void forEveryBlockFromClient    (BLOCKVISITFUNC     func, CBlockVisitor *bv) { forEveryBlockOnDisc        (func, bv, bv->firstBlock(), bv->size()); }
inline void forEveryBlockOnDisc        (BLOCKVISITFUNC     func, CBlockVisitor *bv) { forEveryBlockOnDisc        (func, bv, bv->firstBlock(), bv->size()); }
inline void forEveryNonEmptyBlockOnDisc(BLOCKVISITFUNC     func, CBlockVisitor *bv) { forEveryNonEmptyBlockOnDisc(func, bv, bv->firstBlock(), bv->size()); }
//inline void forEveryFullBlockInMemory  (BLOCKVISITFUNC     func, CBlockVisitor *bv) { forEveryFullBlockInMemory  (func, bv, bv->firstBlock(), bv->size()); }
inline void forEveryMiniBlockInMemory  (MINIBLOCKVISITFUNC func, CBlockVisitor *bv) { forEveryMiniBlockInMemory  (func, bv, bv->firstBlock(), bv->size()); }
extern void endMsg(const string_q& msg="Hit enter to continue...");
