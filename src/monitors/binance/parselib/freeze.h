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
class CFreeze : public CTransaction {
public:
    biguint_t _value;

public:
    CFreeze(void);
    CFreeze(const CFreeze& fr);
    virtual ~CFreeze(void);
    CFreeze& operator=(const CFreeze& fr);

    DECLARE_NODE(CFreeze);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CFreeze& item) const;
    bool operator!=(const CFreeze& item) const { return !operator==(item); }
    friend bool operator<(const CFreeze& v1, const CFreeze& v2);
    friend ostream& operator<<(ostream& os, const CFreeze& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CFreeze& fr);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CFreeze::CFreeze(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CFreeze::CFreeze(const CFreeze& fr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(fr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CFreeze::~CFreeze(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFreeze::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFreeze::initialize(void) {
    CTransaction::initialize();

    _value = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFreeze::duplicate(const CFreeze& fr) {
    clear();
    CTransaction::duplicate(fr);

    _value = fr._value;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CFreeze& CFreeze::operator=(const CFreeze& fr) {
    duplicate(fr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CFreeze::operator==(const CFreeze& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CFreeze& v1, const CFreeze& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CFreeze> CFreezeArray;
extern CArchive& operator>>(CArchive& archive, CFreezeArray& array);
extern CArchive& operator<<(CArchive& archive, const CFreezeArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

