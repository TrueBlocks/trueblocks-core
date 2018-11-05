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
class CRemoveOwnership : public CTransaction {
public:
    biguint_t _dac;

public:
    CRemoveOwnership(void);
    CRemoveOwnership(const CRemoveOwnership& re);
    virtual ~CRemoveOwnership(void);
    CRemoveOwnership& operator=(const CRemoveOwnership& re);

    DECLARE_NODE(CRemoveOwnership);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CRemoveOwnership& item) const;
    bool operator!=(const CRemoveOwnership& item) const { return !operator==(item); }
    friend bool operator<(const CRemoveOwnership& v1, const CRemoveOwnership& v2);
    friend ostream& operator<<(ostream& os, const CRemoveOwnership& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CRemoveOwnership& re);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CRemoveOwnership::CRemoveOwnership(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CRemoveOwnership::CRemoveOwnership(const CRemoveOwnership& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CRemoveOwnership::~CRemoveOwnership(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRemoveOwnership::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRemoveOwnership::initialize(void) {
    CTransaction::initialize();

    _dac = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRemoveOwnership::duplicate(const CRemoveOwnership& re) {
    clear();
    CTransaction::duplicate(re);

    _dac = re._dac;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CRemoveOwnership& CRemoveOwnership::operator=(const CRemoveOwnership& re) {
    duplicate(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CRemoveOwnership::operator==(const CRemoveOwnership& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CRemoveOwnership& v1, const CRemoveOwnership& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CRemoveOwnership> CRemoveOwnershipArray;
extern CArchive& operator>>(CArchive& archive, CRemoveOwnershipArray& array);
extern CArchive& operator<<(CArchive& archive, const CRemoveOwnershipArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

