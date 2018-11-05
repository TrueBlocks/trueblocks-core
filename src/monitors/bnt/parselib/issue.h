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
class CIssue : public CTransaction {
public:
    address_t _to;
    biguint_t _amount;

public:
    CIssue(void);
    CIssue(const CIssue& is);
    virtual ~CIssue(void);
    CIssue& operator=(const CIssue& is);

    DECLARE_NODE(CIssue);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CIssue& item) const;
    bool operator!=(const CIssue& item) const { return !operator==(item); }
    friend bool operator<(const CIssue& v1, const CIssue& v2);
    friend ostream& operator<<(ostream& os, const CIssue& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CIssue& is);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CIssue::CIssue(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CIssue::CIssue(const CIssue& is) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(is);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CIssue::~CIssue(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CIssue::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CIssue::initialize(void) {
    CTransaction::initialize();

    _to = "";
    _amount = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CIssue::duplicate(const CIssue& is) {
    clear();
    CTransaction::duplicate(is);

    _to = is._to;
    _amount = is._amount;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CIssue& CIssue::operator=(const CIssue& is) {
    duplicate(is);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CIssue::operator==(const CIssue& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CIssue& v1, const CIssue& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CIssue> CIssueArray;
extern CArchive& operator>>(CArchive& archive, CIssueArray& array);
extern CArchive& operator<<(CArchive& archive, const CIssueArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

