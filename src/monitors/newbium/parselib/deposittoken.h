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
class CDepositToken : public CTransaction {
public:
    address_t token;
    biguint_t amount;

public:
    CDepositToken(void);
    CDepositToken(const CDepositToken& de);
    virtual ~CDepositToken(void);
    CDepositToken& operator=(const CDepositToken& de);

    DECLARE_NODE(CDepositToken);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CDepositToken& item) const;
    bool operator!=(const CDepositToken& item) const { return !operator==(item); }
    friend bool operator<(const CDepositToken& v1, const CDepositToken& v2);
    friend ostream& operator<<(ostream& os, const CDepositToken& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CDepositToken& de);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CDepositToken::CDepositToken(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CDepositToken::CDepositToken(const CDepositToken& de) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(de);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CDepositToken::~CDepositToken(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDepositToken::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDepositToken::initialize(void) {
    CTransaction::initialize();

    token = "";
    amount = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDepositToken::duplicate(const CDepositToken& de) {
    clear();
    CTransaction::duplicate(de);

    token = de.token;
    amount = de.amount;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CDepositToken& CDepositToken::operator=(const CDepositToken& de) {
    duplicate(de);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CDepositToken::operator==(const CDepositToken& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CDepositToken& v1, const CDepositToken& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CDepositToken> CDepositTokenArray;
extern CArchive& operator>>(CArchive& archive, CDepositTokenArray& array);
extern CArchive& operator<<(CArchive& archive, const CDepositTokenArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

