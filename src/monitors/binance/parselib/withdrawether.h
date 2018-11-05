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
class CWithdrawEther : public CTransaction {
public:
    biguint_t amount;

public:
    CWithdrawEther(void);
    CWithdrawEther(const CWithdrawEther& wi);
    virtual ~CWithdrawEther(void);
    CWithdrawEther& operator=(const CWithdrawEther& wi);

    DECLARE_NODE(CWithdrawEther);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CWithdrawEther& item) const;
    bool operator!=(const CWithdrawEther& item) const { return !operator==(item); }
    friend bool operator<(const CWithdrawEther& v1, const CWithdrawEther& v2);
    friend ostream& operator<<(ostream& os, const CWithdrawEther& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CWithdrawEther& wi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CWithdrawEther::CWithdrawEther(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CWithdrawEther::CWithdrawEther(const CWithdrawEther& wi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(wi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CWithdrawEther::~CWithdrawEther(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CWithdrawEther::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CWithdrawEther::initialize(void) {
    CTransaction::initialize();

    amount = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CWithdrawEther::duplicate(const CWithdrawEther& wi) {
    clear();
    CTransaction::duplicate(wi);

    amount = wi.amount;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CWithdrawEther& CWithdrawEther::operator=(const CWithdrawEther& wi) {
    duplicate(wi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CWithdrawEther::operator==(const CWithdrawEther& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CWithdrawEther& v1, const CWithdrawEther& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CWithdrawEther> CWithdrawEtherArray;
extern CArchive& operator>>(CArchive& archive, CWithdrawEtherArray& array);
extern CArchive& operator<<(CArchive& archive, const CWithdrawEtherArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

