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
class CAcceptOwnership : public CTransaction {
public:
    CAcceptOwnership(void);
    CAcceptOwnership(const CAcceptOwnership& ac);
    virtual ~CAcceptOwnership(void);
    CAcceptOwnership& operator=(const CAcceptOwnership& ac);

    DECLARE_NODE(CAcceptOwnership);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CAcceptOwnership& item) const;
    bool operator!=(const CAcceptOwnership& item) const { return !operator==(item); }
    friend bool operator<(const CAcceptOwnership& v1, const CAcceptOwnership& v2);
    friend ostream& operator<<(ostream& os, const CAcceptOwnership& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CAcceptOwnership& ac);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAcceptOwnership::CAcceptOwnership(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAcceptOwnership::CAcceptOwnership(const CAcceptOwnership& ac) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ac);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAcceptOwnership::~CAcceptOwnership(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAcceptOwnership::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAcceptOwnership::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAcceptOwnership::duplicate(const CAcceptOwnership& ac) {
    clear();
    CTransaction::duplicate(ac);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CAcceptOwnership& CAcceptOwnership::operator=(const CAcceptOwnership& ac) {
    duplicate(ac);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CAcceptOwnership::operator==(const CAcceptOwnership& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CAcceptOwnership& v1, const CAcceptOwnership& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CAcceptOwnership> CAcceptOwnershipArray;
extern CArchive& operator>>(CArchive& archive, CAcceptOwnershipArray& array);
extern CArchive& operator<<(CArchive& archive, const CAcceptOwnershipArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

