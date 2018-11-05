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
class CUnfreeze : public CTransaction {
public:
    biguint_t _value;

public:
    CUnfreeze(void);
    CUnfreeze(const CUnfreeze& un);
    virtual ~CUnfreeze(void);
    CUnfreeze& operator=(const CUnfreeze& un);

    DECLARE_NODE(CUnfreeze);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CUnfreeze& item) const;
    bool operator!=(const CUnfreeze& item) const { return !operator==(item); }
    friend bool operator<(const CUnfreeze& v1, const CUnfreeze& v2);
    friend ostream& operator<<(ostream& os, const CUnfreeze& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CUnfreeze& un);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CUnfreeze::CUnfreeze(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CUnfreeze::CUnfreeze(const CUnfreeze& un) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(un);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CUnfreeze::~CUnfreeze(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CUnfreeze::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CUnfreeze::initialize(void) {
    CTransaction::initialize();

    _value = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CUnfreeze::duplicate(const CUnfreeze& un) {
    clear();
    CTransaction::duplicate(un);

    _value = un._value;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CUnfreeze& CUnfreeze::operator=(const CUnfreeze& un) {
    duplicate(un);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CUnfreeze::operator==(const CUnfreeze& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CUnfreeze& v1, const CUnfreeze& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CUnfreeze> CUnfreezeArray;
extern CArchive& operator>>(CArchive& archive, CUnfreezeArray& array);
extern CArchive& operator<<(CArchive& archive, const CUnfreezeArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

