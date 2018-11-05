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
class CChangeOwner : public CTransaction {
public:
    address_t _newOwner;

public:
    CChangeOwner(void);
    CChangeOwner(const CChangeOwner& ch);
    virtual ~CChangeOwner(void);
    CChangeOwner& operator=(const CChangeOwner& ch);

    DECLARE_NODE(CChangeOwner);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CChangeOwner& item) const;
    bool operator!=(const CChangeOwner& item) const { return !operator==(item); }
    friend bool operator<(const CChangeOwner& v1, const CChangeOwner& v2);
    friend ostream& operator<<(ostream& os, const CChangeOwner& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CChangeOwner& ch);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CChangeOwner::CChangeOwner(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CChangeOwner::CChangeOwner(const CChangeOwner& ch) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ch);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CChangeOwner::~CChangeOwner(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeOwner::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeOwner::initialize(void) {
    CTransaction::initialize();

    _newOwner = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeOwner::duplicate(const CChangeOwner& ch) {
    clear();
    CTransaction::duplicate(ch);

    _newOwner = ch._newOwner;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CChangeOwner& CChangeOwner::operator=(const CChangeOwner& ch) {
    duplicate(ch);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CChangeOwner::operator==(const CChangeOwner& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CChangeOwner& v1, const CChangeOwner& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CChangeOwner> CChangeOwnerArray;
extern CArchive& operator>>(CArchive& archive, CChangeOwnerArray& array);
extern CArchive& operator<<(CArchive& archive, const CChangeOwnerArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

