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
class CIssueTokens : public CTransaction {
public:
    address_t _to;
    biguint_t _amount;

public:
    CIssueTokens(void);
    CIssueTokens(const CIssueTokens& is);
    virtual ~CIssueTokens(void);
    CIssueTokens& operator=(const CIssueTokens& is);

    DECLARE_NODE(CIssueTokens);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CIssueTokens& item) const;
    bool operator!=(const CIssueTokens& item) const { return !operator==(item); }
    friend bool operator<(const CIssueTokens& v1, const CIssueTokens& v2);
    friend ostream& operator<<(ostream& os, const CIssueTokens& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CIssueTokens& is);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CIssueTokens::CIssueTokens(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CIssueTokens::CIssueTokens(const CIssueTokens& is) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(is);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CIssueTokens::~CIssueTokens(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CIssueTokens::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CIssueTokens::initialize(void) {
    CTransaction::initialize();

    _to = "";
    _amount = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CIssueTokens::duplicate(const CIssueTokens& is) {
    clear();
    CTransaction::duplicate(is);

    _to = is._to;
    _amount = is._amount;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CIssueTokens& CIssueTokens::operator=(const CIssueTokens& is) {
    duplicate(is);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CIssueTokens::operator==(const CIssueTokens& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CIssueTokens& v1, const CIssueTokens& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CIssueTokens> CIssueTokensArray;
extern CArchive& operator>>(CArchive& archive, CIssueTokensArray& array);
extern CArchive& operator<<(CArchive& archive, const CIssueTokensArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

