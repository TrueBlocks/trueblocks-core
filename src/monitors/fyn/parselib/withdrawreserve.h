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
class CWithdrawReserve : public CTransaction {
public:
    address_t _beneficiary;

public:
    CWithdrawReserve(void);
    CWithdrawReserve(const CWithdrawReserve& wi);
    virtual ~CWithdrawReserve(void);
    CWithdrawReserve& operator=(const CWithdrawReserve& wi);

    DECLARE_NODE(CWithdrawReserve);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CWithdrawReserve& item) const;
    bool operator!=(const CWithdrawReserve& item) const { return !operator==(item); }
    friend bool operator<(const CWithdrawReserve& v1, const CWithdrawReserve& v2);
    friend ostream& operator<<(ostream& os, const CWithdrawReserve& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CWithdrawReserve& wi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CWithdrawReserve::CWithdrawReserve(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CWithdrawReserve::CWithdrawReserve(const CWithdrawReserve& wi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(wi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CWithdrawReserve::~CWithdrawReserve(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CWithdrawReserve::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CWithdrawReserve::initialize(void) {
    CTransaction::initialize();

    _beneficiary = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CWithdrawReserve::duplicate(const CWithdrawReserve& wi) {
    clear();
    CTransaction::duplicate(wi);

    _beneficiary = wi._beneficiary;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CWithdrawReserve& CWithdrawReserve::operator=(const CWithdrawReserve& wi) {
    duplicate(wi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CWithdrawReserve::operator==(const CWithdrawReserve& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CWithdrawReserve& v1, const CWithdrawReserve& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CWithdrawReserve> CWithdrawReserveArray;
extern CArchive& operator>>(CArchive& archive, CWithdrawReserveArray& array);
extern CArchive& operator<<(CArchive& archive, const CWithdrawReserveArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

