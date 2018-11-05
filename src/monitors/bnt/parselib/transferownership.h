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
class CTransferOwnership : public CTransaction {
public:
    address_t _newOwner;

public:
    CTransferOwnership(void);
    CTransferOwnership(const CTransferOwnership& tr);
    virtual ~CTransferOwnership(void);
    CTransferOwnership& operator=(const CTransferOwnership& tr);

    DECLARE_NODE(CTransferOwnership);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CTransferOwnership& item) const;
    bool operator!=(const CTransferOwnership& item) const { return !operator==(item); }
    friend bool operator<(const CTransferOwnership& v1, const CTransferOwnership& v2);
    friend ostream& operator<<(ostream& os, const CTransferOwnership& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CTransferOwnership& tr);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTransferOwnership::CTransferOwnership(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTransferOwnership::CTransferOwnership(const CTransferOwnership& tr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(tr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTransferOwnership::~CTransferOwnership(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTransferOwnership::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTransferOwnership::initialize(void) {
    CTransaction::initialize();

    _newOwner = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTransferOwnership::duplicate(const CTransferOwnership& tr) {
    clear();
    CTransaction::duplicate(tr);

    _newOwner = tr._newOwner;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CTransferOwnership& CTransferOwnership::operator=(const CTransferOwnership& tr) {
    duplicate(tr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CTransferOwnership::operator==(const CTransferOwnership& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CTransferOwnership& v1, const CTransferOwnership& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CTransferOwnership> CTransferOwnershipArray;
extern CArchive& operator>>(CArchive& archive, CTransferOwnershipArray& array);
extern CArchive& operator<<(CArchive& archive, const CTransferOwnershipArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

