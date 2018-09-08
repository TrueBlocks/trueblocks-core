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
class CFinalizeAuction : public CTransaction {
public:
    string_q _hash;

public:
    CFinalizeAuction(void);
    CFinalizeAuction(const CFinalizeAuction& fi);
    virtual ~CFinalizeAuction(void);
    CFinalizeAuction& operator=(const CFinalizeAuction& fi);

    DECLARE_NODE(CFinalizeAuction);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CFinalizeAuction& item) const;
    bool operator!=(const CFinalizeAuction& item) const { return !operator==(item); }
    friend bool operator<(const CFinalizeAuction& v1, const CFinalizeAuction& v2);
    friend ostream& operator<<(ostream& os, const CFinalizeAuction& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CFinalizeAuction& fi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CFinalizeAuction::CFinalizeAuction(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CFinalizeAuction::CFinalizeAuction(const CFinalizeAuction& fi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(fi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CFinalizeAuction::~CFinalizeAuction(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFinalizeAuction::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFinalizeAuction::initialize(void) {
    CTransaction::initialize();

    _hash = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFinalizeAuction::duplicate(const CFinalizeAuction& fi) {
    clear();
    CTransaction::duplicate(fi);

    _hash = fi._hash;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CFinalizeAuction& CFinalizeAuction::operator=(const CFinalizeAuction& fi) {
    duplicate(fi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CFinalizeAuction::operator==(const CFinalizeAuction& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CFinalizeAuction& v1, const CFinalizeAuction& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CFinalizeAuction> CFinalizeAuctionArray;
extern CArchive& operator>>(CArchive& archive, CFinalizeAuctionArray& array);
extern CArchive& operator<<(CArchive& archive, const CFinalizeAuctionArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

