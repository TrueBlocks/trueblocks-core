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
class CDecreaseApproval : public CTransaction {
public:
    address_t _spender;
    biguint_t _subtractedValue;

public:
    CDecreaseApproval(void);
    CDecreaseApproval(const CDecreaseApproval& de);
    virtual ~CDecreaseApproval(void);
    CDecreaseApproval& operator=(const CDecreaseApproval& de);

    DECLARE_NODE(CDecreaseApproval);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CDecreaseApproval& item) const;
    bool operator!=(const CDecreaseApproval& item) const { return !operator==(item); }
    friend bool operator<(const CDecreaseApproval& v1, const CDecreaseApproval& v2);
    friend ostream& operator<<(ostream& os, const CDecreaseApproval& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CDecreaseApproval& de);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CDecreaseApproval::CDecreaseApproval(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CDecreaseApproval::CDecreaseApproval(const CDecreaseApproval& de) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(de);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CDecreaseApproval::~CDecreaseApproval(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDecreaseApproval::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDecreaseApproval::initialize(void) {
    CTransaction::initialize();

    _spender = "";
    _subtractedValue = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDecreaseApproval::duplicate(const CDecreaseApproval& de) {
    clear();
    CTransaction::duplicate(de);

    _spender = de._spender;
    _subtractedValue = de._subtractedValue;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CDecreaseApproval& CDecreaseApproval::operator=(const CDecreaseApproval& de) {
    duplicate(de);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CDecreaseApproval::operator==(const CDecreaseApproval& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CDecreaseApproval& v1, const CDecreaseApproval& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CDecreaseApproval> CDecreaseApprovalArray;
extern CArchive& operator>>(CArchive& archive, CDecreaseApprovalArray& array);
extern CArchive& operator<<(CArchive& archive, const CDecreaseApprovalArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

