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
class CDestroy : public CTransaction {
public:
    address_t _from;
    biguint_t _amount;

public:
    CDestroy(void);
    CDestroy(const CDestroy& de);
    virtual ~CDestroy(void);
    CDestroy& operator=(const CDestroy& de);

    DECLARE_NODE(CDestroy);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CDestroy& item) const;
    bool operator!=(const CDestroy& item) const { return !operator==(item); }
    friend bool operator<(const CDestroy& v1, const CDestroy& v2);
    friend ostream& operator<<(ostream& os, const CDestroy& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CDestroy& de);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CDestroy::CDestroy(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CDestroy::CDestroy(const CDestroy& de) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(de);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CDestroy::~CDestroy(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDestroy::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDestroy::initialize(void) {
    CTransaction::initialize();

    _from = "";
    _amount = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDestroy::duplicate(const CDestroy& de) {
    clear();
    CTransaction::duplicate(de);

    _from = de._from;
    _amount = de._amount;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CDestroy& CDestroy::operator=(const CDestroy& de) {
    duplicate(de);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CDestroy::operator==(const CDestroy& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CDestroy& v1, const CDestroy& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CDestroy> CDestroyArray;
extern CArchive& operator>>(CArchive& archive, CDestroyArray& array);
extern CArchive& operator<<(CArchive& archive, const CDestroyArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

