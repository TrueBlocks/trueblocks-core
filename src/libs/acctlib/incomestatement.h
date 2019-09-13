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
/*
 * This file was generated with makeClass. Edit only those parts of the code inside
 * of 'EXISTING_CODE' tags.
 */
#include "etherlib.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CIncomeStatement : public CBaseNode {
public:
    bigint_t begBal;
    bigint_t inflow;
    bigint_t outflow;
    bigint_t gasCostInWei;
    bigint_t endBal;
    blknum_t blockNum;

public:
    CIncomeStatement(void);
    CIncomeStatement(const CIncomeStatement& in);
    virtual ~CIncomeStatement(void);
    CIncomeStatement& operator=(const CIncomeStatement& in);

    DECLARE_NODE(CIncomeStatement);

    // EXISTING_CODE
    bigint_t curBalance;
    void operator+=(const CIncomeStatement &x);
    bool balanced(void) const { return ((curBalance - endBal) == 0); }
    bigint_t difference(void) const { return (curBalance - endBal); }
    void correct(void) { endBal = curBalance; }
    friend class CAccountWatch;
    // EXISTING_CODE
    bool operator==(const CIncomeStatement& item) const;
    bool operator!=(const CIncomeStatement& item) const { return !operator==(item); }
    friend bool operator<(const CIncomeStatement& v1, const CIncomeStatement& v2);
    friend ostream& operator<<(ostream& os, const CIncomeStatement& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CIncomeStatement& in);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CIncomeStatement::CIncomeStatement(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CIncomeStatement::CIncomeStatement(const CIncomeStatement& in) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(in);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CIncomeStatement::~CIncomeStatement(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CIncomeStatement::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CIncomeStatement::initialize(void) {
    CBaseNode::initialize();

    begBal = 0;
    inflow = 0;
    outflow = 0;
    gasCostInWei = 0;
    endBal = 0;
    blockNum = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CIncomeStatement::duplicate(const CIncomeStatement& in) {
    clear();
    CBaseNode::duplicate(in);

    begBal = in.begBal;
    inflow = in.inflow;
    outflow = in.outflow;
    gasCostInWei = in.gasCostInWei;
    endBal = in.endBal;
    blockNum = in.blockNum;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CIncomeStatement& CIncomeStatement::operator=(const CIncomeStatement& in) {
    duplicate(in);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CIncomeStatement::operator==(const CIncomeStatement& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CIncomeStatement& v1, const CIncomeStatement& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CIncomeStatement> CIncomeStatementArray;
extern CArchive& operator>>(CArchive& archive, CIncomeStatementArray& array);
extern CArchive& operator<<(CArchive& archive, const CIncomeStatementArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CIncomeStatement& inc);
extern CArchive& operator>>(CArchive& archive, CIncomeStatement& inc);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_INCOMESTATEMENT;

//---------------------------------------------------------------------------
// EXISTING_CODE
//------------------------------------------------------------
inline void CIncomeStatement::operator+=(const CIncomeStatement &x) {
    begBal += x.begBal;
    inflow += x.inflow;
    outflow += x.outflow;
    gasCostInWei += x.gasCostInWei;
    endBal += x.endBal;
    blockNum = x.blockNum;
}
// EXISTING_CODE
}  // namespace qblocks

