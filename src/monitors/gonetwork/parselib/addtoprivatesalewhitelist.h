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
class CAddToPrivateSaleWhitelist : public CTransaction {
public:
    CAddressArray _bidder_addresses;

public:
    CAddToPrivateSaleWhitelist(void);
    CAddToPrivateSaleWhitelist(const CAddToPrivateSaleWhitelist& ad);
    virtual ~CAddToPrivateSaleWhitelist(void);
    CAddToPrivateSaleWhitelist& operator=(const CAddToPrivateSaleWhitelist& ad);

    DECLARE_NODE(CAddToPrivateSaleWhitelist);

    const string_q getStringAt(const string_q& name, size_t i) const override;

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CAddToPrivateSaleWhitelist& item) const;
    bool operator!=(const CAddToPrivateSaleWhitelist& item) const { return !operator==(item); }
    friend bool operator<(const CAddToPrivateSaleWhitelist& v1, const CAddToPrivateSaleWhitelist& v2);
    friend ostream& operator<<(ostream& os, const CAddToPrivateSaleWhitelist& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CAddToPrivateSaleWhitelist& ad);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAddToPrivateSaleWhitelist::CAddToPrivateSaleWhitelist(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAddToPrivateSaleWhitelist::CAddToPrivateSaleWhitelist(const CAddToPrivateSaleWhitelist& ad) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ad);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAddToPrivateSaleWhitelist::~CAddToPrivateSaleWhitelist(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAddToPrivateSaleWhitelist::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAddToPrivateSaleWhitelist::initialize(void) {
    CTransaction::initialize();

    _bidder_addresses.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAddToPrivateSaleWhitelist::duplicate(const CAddToPrivateSaleWhitelist& ad) {
    clear();
    CTransaction::duplicate(ad);

    _bidder_addresses = ad._bidder_addresses;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CAddToPrivateSaleWhitelist& CAddToPrivateSaleWhitelist::operator=(const CAddToPrivateSaleWhitelist& ad) {
    duplicate(ad);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CAddToPrivateSaleWhitelist::operator==(const CAddToPrivateSaleWhitelist& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CAddToPrivateSaleWhitelist& v1, const CAddToPrivateSaleWhitelist& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CAddToPrivateSaleWhitelist> CAddToPrivateSaleWhitelistArray;
extern CArchive& operator>>(CArchive& archive, CAddToPrivateSaleWhitelistArray& array);
extern CArchive& operator<<(CArchive& archive, const CAddToPrivateSaleWhitelistArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

