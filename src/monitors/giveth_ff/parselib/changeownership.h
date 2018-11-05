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
class CChangeOwnership : public CTransaction {
public:
    address_t _newOwner;

public:
    CChangeOwnership(void);
    CChangeOwnership(const CChangeOwnership& ch);
    virtual ~CChangeOwnership(void);
    CChangeOwnership& operator=(const CChangeOwnership& ch);

    DECLARE_NODE(CChangeOwnership);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CChangeOwnership& item) const;
    bool operator!=(const CChangeOwnership& item) const { return !operator==(item); }
    friend bool operator<(const CChangeOwnership& v1, const CChangeOwnership& v2);
    friend ostream& operator<<(ostream& os, const CChangeOwnership& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CChangeOwnership& ch);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CChangeOwnership::CChangeOwnership(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CChangeOwnership::CChangeOwnership(const CChangeOwnership& ch) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ch);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CChangeOwnership::~CChangeOwnership(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeOwnership::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeOwnership::initialize(void) {
    CTransaction::initialize();

    _newOwner = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeOwnership::duplicate(const CChangeOwnership& ch) {
    clear();
    CTransaction::duplicate(ch);

    _newOwner = ch._newOwner;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CChangeOwnership& CChangeOwnership::operator=(const CChangeOwnership& ch) {
    duplicate(ch);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CChangeOwnership::operator==(const CChangeOwnership& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CChangeOwnership& v1, const CChangeOwnership& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CChangeOwnership> CChangeOwnershipArray;
extern CArchive& operator>>(CArchive& archive, CChangeOwnershipArray& array);
extern CArchive& operator<<(CArchive& archive, const CChangeOwnershipArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

