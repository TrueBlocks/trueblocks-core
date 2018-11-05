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
class CIncreaseApproval : public CTransaction {
public:
    address_t _spender;
    biguint_t _addedValue;

public:
    CIncreaseApproval(void);
    CIncreaseApproval(const CIncreaseApproval& in);
    virtual ~CIncreaseApproval(void);
    CIncreaseApproval& operator=(const CIncreaseApproval& in);

    DECLARE_NODE(CIncreaseApproval);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CIncreaseApproval& item) const;
    bool operator!=(const CIncreaseApproval& item) const { return !operator==(item); }
    friend bool operator<(const CIncreaseApproval& v1, const CIncreaseApproval& v2);
    friend ostream& operator<<(ostream& os, const CIncreaseApproval& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CIncreaseApproval& in);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CIncreaseApproval::CIncreaseApproval(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CIncreaseApproval::CIncreaseApproval(const CIncreaseApproval& in) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(in);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CIncreaseApproval::~CIncreaseApproval(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CIncreaseApproval::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CIncreaseApproval::initialize(void) {
    CTransaction::initialize();

    _spender = "";
    _addedValue = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CIncreaseApproval::duplicate(const CIncreaseApproval& in) {
    clear();
    CTransaction::duplicate(in);

    _spender = in._spender;
    _addedValue = in._addedValue;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CIncreaseApproval& CIncreaseApproval::operator=(const CIncreaseApproval& in) {
    duplicate(in);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CIncreaseApproval::operator==(const CIncreaseApproval& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CIncreaseApproval& v1, const CIncreaseApproval& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CIncreaseApproval> CIncreaseApprovalArray;
extern CArchive& operator>>(CArchive& archive, CIncreaseApprovalArray& array);
extern CArchive& operator<<(CArchive& archive, const CIncreaseApprovalArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

