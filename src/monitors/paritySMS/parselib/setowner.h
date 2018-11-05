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
class CSetOwner : public CTransaction {
public:
    address_t _new;

public:
    CSetOwner(void);
    CSetOwner(const CSetOwner& se);
    virtual ~CSetOwner(void);
    CSetOwner& operator=(const CSetOwner& se);

    DECLARE_NODE(CSetOwner);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CSetOwner& item) const;
    bool operator!=(const CSetOwner& item) const { return !operator==(item); }
    friend bool operator<(const CSetOwner& v1, const CSetOwner& v2);
    friend ostream& operator<<(ostream& os, const CSetOwner& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CSetOwner& se);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CSetOwner::CSetOwner(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CSetOwner::CSetOwner(const CSetOwner& se) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(se);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CSetOwner::~CSetOwner(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetOwner::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetOwner::initialize(void) {
    CTransaction::initialize();

    _new = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetOwner::duplicate(const CSetOwner& se) {
    clear();
    CTransaction::duplicate(se);

    _new = se._new;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CSetOwner& CSetOwner::operator=(const CSetOwner& se) {
    duplicate(se);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CSetOwner::operator==(const CSetOwner& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CSetOwner& v1, const CSetOwner& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CSetOwner> CSetOwnerArray;
extern CArchive& operator>>(CArchive& archive, CSetOwnerArray& array);
extern CArchive& operator<<(CArchive& archive, const CSetOwnerArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

