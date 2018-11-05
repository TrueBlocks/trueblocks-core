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
class CAcceptTokenOwnership : public CTransaction {
public:
    CAcceptTokenOwnership(void);
    CAcceptTokenOwnership(const CAcceptTokenOwnership& ac);
    virtual ~CAcceptTokenOwnership(void);
    CAcceptTokenOwnership& operator=(const CAcceptTokenOwnership& ac);

    DECLARE_NODE(CAcceptTokenOwnership);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CAcceptTokenOwnership& item) const;
    bool operator!=(const CAcceptTokenOwnership& item) const { return !operator==(item); }
    friend bool operator<(const CAcceptTokenOwnership& v1, const CAcceptTokenOwnership& v2);
    friend ostream& operator<<(ostream& os, const CAcceptTokenOwnership& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CAcceptTokenOwnership& ac);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAcceptTokenOwnership::CAcceptTokenOwnership(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAcceptTokenOwnership::CAcceptTokenOwnership(const CAcceptTokenOwnership& ac) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ac);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAcceptTokenOwnership::~CAcceptTokenOwnership(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAcceptTokenOwnership::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAcceptTokenOwnership::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAcceptTokenOwnership::duplicate(const CAcceptTokenOwnership& ac) {
    clear();
    CTransaction::duplicate(ac);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CAcceptTokenOwnership& CAcceptTokenOwnership::operator=(const CAcceptTokenOwnership& ac) {
    duplicate(ac);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CAcceptTokenOwnership::operator==(const CAcceptTokenOwnership& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CAcceptTokenOwnership& v1, const CAcceptTokenOwnership& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CAcceptTokenOwnership> CAcceptTokenOwnershipArray;
extern CArchive& operator>>(CArchive& archive, CAcceptTokenOwnershipArray& array);
extern CArchive& operator<<(CArchive& archive, const CAcceptTokenOwnershipArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

