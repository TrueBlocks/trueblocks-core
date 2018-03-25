#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
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

