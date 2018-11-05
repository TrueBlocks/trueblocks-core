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
class CWithdrawTokens : public CTransaction {
public:
    address_t _token;
    address_t _to;
    biguint_t _amount;

public:
    CWithdrawTokens(void);
    CWithdrawTokens(const CWithdrawTokens& wi);
    virtual ~CWithdrawTokens(void);
    CWithdrawTokens& operator=(const CWithdrawTokens& wi);

    DECLARE_NODE(CWithdrawTokens);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CWithdrawTokens& item) const;
    bool operator!=(const CWithdrawTokens& item) const { return !operator==(item); }
    friend bool operator<(const CWithdrawTokens& v1, const CWithdrawTokens& v2);
    friend ostream& operator<<(ostream& os, const CWithdrawTokens& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CWithdrawTokens& wi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CWithdrawTokens::CWithdrawTokens(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CWithdrawTokens::CWithdrawTokens(const CWithdrawTokens& wi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(wi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CWithdrawTokens::~CWithdrawTokens(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CWithdrawTokens::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CWithdrawTokens::initialize(void) {
    CTransaction::initialize();

    _token = "";
    _to = "";
    _amount = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CWithdrawTokens::duplicate(const CWithdrawTokens& wi) {
    clear();
    CTransaction::duplicate(wi);

    _token = wi._token;
    _to = wi._to;
    _amount = wi._amount;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CWithdrawTokens& CWithdrawTokens::operator=(const CWithdrawTokens& wi) {
    duplicate(wi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CWithdrawTokens::operator==(const CWithdrawTokens& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CWithdrawTokens& v1, const CWithdrawTokens& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CWithdrawTokens> CWithdrawTokensArray;
extern CArchive& operator>>(CArchive& archive, CWithdrawTokensArray& array);
extern CArchive& operator<<(CArchive& archive, const CWithdrawTokensArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

