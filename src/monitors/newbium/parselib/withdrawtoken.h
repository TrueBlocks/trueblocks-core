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
class CWithdrawToken : public CTransaction {
public:
    address_t token;
    biguint_t amount;

public:
    CWithdrawToken(void);
    CWithdrawToken(const CWithdrawToken& wi);
    virtual ~CWithdrawToken(void);
    CWithdrawToken& operator=(const CWithdrawToken& wi);

    DECLARE_NODE(CWithdrawToken);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CWithdrawToken& item) const;
    bool operator!=(const CWithdrawToken& item) const { return !operator==(item); }
    friend bool operator<(const CWithdrawToken& v1, const CWithdrawToken& v2);
    friend ostream& operator<<(ostream& os, const CWithdrawToken& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CWithdrawToken& wi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CWithdrawToken::CWithdrawToken(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CWithdrawToken::CWithdrawToken(const CWithdrawToken& wi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(wi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CWithdrawToken::~CWithdrawToken(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CWithdrawToken::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CWithdrawToken::initialize(void) {
    CTransaction::initialize();

    token = "";
    amount = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CWithdrawToken::duplicate(const CWithdrawToken& wi) {
    clear();
    CTransaction::duplicate(wi);

    token = wi.token;
    amount = wi.amount;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CWithdrawToken& CWithdrawToken::operator=(const CWithdrawToken& wi) {
    duplicate(wi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CWithdrawToken::operator==(const CWithdrawToken& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CWithdrawToken& v1, const CWithdrawToken& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CWithdrawToken> CWithdrawTokenArray;
extern CArchive& operator>>(CArchive& archive, CWithdrawTokenArray& array);
extern CArchive& operator<<(CArchive& archive, const CWithdrawTokenArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

