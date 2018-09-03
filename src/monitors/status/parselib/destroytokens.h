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
class CDestroyTokens : public CTransaction {
public:
    address_t _owner;
    biguint_t _amount;

public:
    CDestroyTokens(void);
    CDestroyTokens(const CDestroyTokens& de);
    virtual ~CDestroyTokens(void);
    CDestroyTokens& operator=(const CDestroyTokens& de);

    DECLARE_NODE(CDestroyTokens);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CDestroyTokens& item) const;
    bool operator!=(const CDestroyTokens& item) const { return !operator==(item); }
    friend bool operator<(const CDestroyTokens& v1, const CDestroyTokens& v2);
    friend ostream& operator<<(ostream& os, const CDestroyTokens& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CDestroyTokens& de);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CDestroyTokens::CDestroyTokens(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CDestroyTokens::CDestroyTokens(const CDestroyTokens& de) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(de);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CDestroyTokens::~CDestroyTokens(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDestroyTokens::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDestroyTokens::initialize(void) {
    CTransaction::initialize();

    _owner = "";
    _amount = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDestroyTokens::duplicate(const CDestroyTokens& de) {
    clear();
    CTransaction::duplicate(de);

    _owner = de._owner;
    _amount = de._amount;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CDestroyTokens& CDestroyTokens::operator=(const CDestroyTokens& de) {
    duplicate(de);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CDestroyTokens::operator==(const CDestroyTokens& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CDestroyTokens& v1, const CDestroyTokens& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CDestroyTokens> CDestroyTokensArray;
extern CArchive& operator>>(CArchive& archive, CDestroyTokensArray& array);
extern CArchive& operator<<(CArchive& archive, const CDestroyTokensArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

