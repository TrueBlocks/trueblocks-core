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
class CWithdrawFunding : public CTransaction {
public:
    CWithdrawFunding(void);
    CWithdrawFunding(const CWithdrawFunding& wi);
    virtual ~CWithdrawFunding(void);
    CWithdrawFunding& operator=(const CWithdrawFunding& wi);

    DECLARE_NODE(CWithdrawFunding);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CWithdrawFunding& item) const;
    bool operator!=(const CWithdrawFunding& item) const { return !operator==(item); }
    friend bool operator<(const CWithdrawFunding& v1, const CWithdrawFunding& v2);
    friend ostream& operator<<(ostream& os, const CWithdrawFunding& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CWithdrawFunding& wi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CWithdrawFunding::CWithdrawFunding(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CWithdrawFunding::CWithdrawFunding(const CWithdrawFunding& wi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(wi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CWithdrawFunding::~CWithdrawFunding(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CWithdrawFunding::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CWithdrawFunding::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CWithdrawFunding::duplicate(const CWithdrawFunding& wi) {
    clear();
    CTransaction::duplicate(wi);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CWithdrawFunding& CWithdrawFunding::operator=(const CWithdrawFunding& wi) {
    duplicate(wi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CWithdrawFunding::operator==(const CWithdrawFunding& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CWithdrawFunding& v1, const CWithdrawFunding& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CWithdrawFunding> CWithdrawFundingArray;
extern CArchive& operator>>(CArchive& archive, CWithdrawFundingArray& array);
extern CArchive& operator<<(CArchive& archive, const CWithdrawFundingArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

