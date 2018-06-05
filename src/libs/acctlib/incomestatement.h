#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
/*
 * This file was generated with makeClass. Edit only those parts of the code inside
 * of 'EXISTING_CODE' tags.
 */
#include "abilib.h"

namespace qblocks {

//--------------------------------------------------------------------------
class CIncomeStatement;
typedef SFArrayBase<CIncomeStatement>         CIncomeStatementArray;
typedef SFList<CIncomeStatement*>             CIncomeStatementList;
typedef SFUniqueList<CIncomeStatement*>       CIncomeStatementListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CIncomeStatement : public CBaseNode {
public:
    SFIntBN begBal;
    SFIntBN inflow;
    SFIntBN outflow;
    SFIntBN gasCost;
    SFIntBN endBal;
    blknum_t blockNum;

public:
    CIncomeStatement(void);
    CIncomeStatement(const CIncomeStatement& in);
    virtual ~CIncomeStatement(void);
    CIncomeStatement& operator=(const CIncomeStatement& in);

    DECLARE_NODE(CIncomeStatement);

    // EXISTING_CODE
    SFIntBN nodeBal;
    void operator+=(const CIncomeStatement &x);
    bool balanced(void) const { return ((nodeBal - endBal) == 0); }
    SFIntBN difference(void) const { return (nodeBal - endBal); }
    void correct(void) { endBal = nodeBal; }
    friend class CAccountWatch;
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const CIncomeStatement& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CIncomeStatement& in);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CIncomeStatement::CIncomeStatement(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CIncomeStatement::CIncomeStatement(const CIncomeStatement& in) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(in);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CIncomeStatement::~CIncomeStatement(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CIncomeStatement::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CIncomeStatement::Init(void) {
    CBaseNode::Init();

    begBal = 0;
    inflow = 0;
    outflow = 0;
    gasCost = 0;
    endBal = 0;
    blockNum = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CIncomeStatement::Copy(const CIncomeStatement& in) {
    Clear();
    CBaseNode::Copy(in);

    begBal = in.begBal;
    inflow = in.inflow;
    outflow = in.outflow;
    gasCost = in.gasCost;
    endBal = in.endBal;
    blockNum = in.blockNum;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CIncomeStatement& CIncomeStatement::operator=(const CIncomeStatement& in) {
    Copy(in);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CIncomeStatementArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CIncomeStatementArray);
IMPLEMENT_ARCHIVE_LIST(CIncomeStatementList);

//---------------------------------------------------------------------------
extern SFArchive& operator<<(SFArchive& archive, const CIncomeStatement& inc);
extern SFArchive& operator>>(SFArchive& archive, CIncomeStatement& inc);

//---------------------------------------------------------------------------
// EXISTING_CODE
//------------------------------------------------------------
inline void CIncomeStatement::operator+=(const CIncomeStatement &x)
{
    begBal += x.begBal;
    inflow += x.inflow;
    outflow += x.outflow;
    gasCost += x.gasCost;
    endBal += x.endBal;
    blockNum = x.blockNum;
}
// EXISTING_CODE
}  // namespace qblocks

