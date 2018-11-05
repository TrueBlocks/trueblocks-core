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
class CSetAuthority : public CTransaction {
public:
    address_t authority_;

public:
    CSetAuthority(void);
    CSetAuthority(const CSetAuthority& se);
    virtual ~CSetAuthority(void);
    CSetAuthority& operator=(const CSetAuthority& se);

    DECLARE_NODE(CSetAuthority);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CSetAuthority& item) const;
    bool operator!=(const CSetAuthority& item) const { return !operator==(item); }
    friend bool operator<(const CSetAuthority& v1, const CSetAuthority& v2);
    friend ostream& operator<<(ostream& os, const CSetAuthority& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CSetAuthority& se);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CSetAuthority::CSetAuthority(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CSetAuthority::CSetAuthority(const CSetAuthority& se) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(se);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CSetAuthority::~CSetAuthority(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetAuthority::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetAuthority::initialize(void) {
    CTransaction::initialize();

    authority_ = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetAuthority::duplicate(const CSetAuthority& se) {
    clear();
    CTransaction::duplicate(se);

    authority_ = se.authority_;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CSetAuthority& CSetAuthority::operator=(const CSetAuthority& se) {
    duplicate(se);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CSetAuthority::operator==(const CSetAuthority& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CSetAuthority& v1, const CSetAuthority& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CSetAuthority> CSetAuthorityArray;
extern CArchive& operator>>(CArchive& archive, CSetAuthorityArray& array);
extern CArchive& operator<<(CArchive& archive, const CSetAuthorityArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

