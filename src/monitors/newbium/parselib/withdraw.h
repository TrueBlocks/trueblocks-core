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
#include <vector>
#include <map>
#include "transaction.h"

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CWithdraw : public CTransaction {
public:
    biguint_t amount;

public:
    CWithdraw(void);
    CWithdraw(const CWithdraw& wi);
    virtual ~CWithdraw(void);
    CWithdraw& operator=(const CWithdraw& wi);

    DECLARE_NODE(CWithdraw);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CWithdraw& item) const;
    bool operator!=(const CWithdraw& item) const { return !operator==(item); }
    friend bool operator<(const CWithdraw& v1, const CWithdraw& v2);
    friend ostream& operator<<(ostream& os, const CWithdraw& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CWithdraw& wi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CWithdraw::CWithdraw(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CWithdraw::CWithdraw(const CWithdraw& wi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(wi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CWithdraw::~CWithdraw(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CWithdraw::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CWithdraw::initialize(void) {
    CTransaction::initialize();

    amount = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CWithdraw::duplicate(const CWithdraw& wi) {
    clear();
    CTransaction::duplicate(wi);

    amount = wi.amount;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CWithdraw& CWithdraw::operator=(const CWithdraw& wi) {
    duplicate(wi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CWithdraw::operator==(const CWithdraw& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CWithdraw& v1, const CWithdraw& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CWithdraw> CWithdrawArray;
extern CArchive& operator>>(CArchive& archive, CWithdrawArray& array);
extern CArchive& operator<<(CArchive& archive, const CWithdrawArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

