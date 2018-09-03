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
class CStartAuction : public CTransaction {
public:
    string_q _hash;

public:
    CStartAuction(void);
    CStartAuction(const CStartAuction& st);
    virtual ~CStartAuction(void);
    CStartAuction& operator=(const CStartAuction& st);

    DECLARE_NODE(CStartAuction);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CStartAuction& item) const;
    bool operator!=(const CStartAuction& item) const { return !operator==(item); }
    friend bool operator<(const CStartAuction& v1, const CStartAuction& v2);
    friend ostream& operator<<(ostream& os, const CStartAuction& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CStartAuction& st);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CStartAuction::CStartAuction(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CStartAuction::CStartAuction(const CStartAuction& st) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(st);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CStartAuction::~CStartAuction(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CStartAuction::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CStartAuction::initialize(void) {
    CTransaction::initialize();

    _hash = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CStartAuction::duplicate(const CStartAuction& st) {
    clear();
    CTransaction::duplicate(st);

    _hash = st._hash;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CStartAuction& CStartAuction::operator=(const CStartAuction& st) {
    duplicate(st);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CStartAuction::operator==(const CStartAuction& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CStartAuction& v1, const CStartAuction& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CStartAuction> CStartAuctionArray;
extern CArchive& operator>>(CArchive& archive, CStartAuctionArray& array);
extern CArchive& operator<<(CArchive& archive, const CStartAuctionArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

