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
class CAddToPublicSaleWhitelist : public CTransaction {
public:
    CAddressArray _bidder_addresses;

public:
    CAddToPublicSaleWhitelist(void);
    CAddToPublicSaleWhitelist(const CAddToPublicSaleWhitelist& ad);
    virtual ~CAddToPublicSaleWhitelist(void);
    CAddToPublicSaleWhitelist& operator=(const CAddToPublicSaleWhitelist& ad);

    DECLARE_NODE(CAddToPublicSaleWhitelist);

    const string_q getStringAt(const string_q& name, size_t i) const override;

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CAddToPublicSaleWhitelist& item) const;
    bool operator!=(const CAddToPublicSaleWhitelist& item) const { return !operator==(item); }
    friend bool operator<(const CAddToPublicSaleWhitelist& v1, const CAddToPublicSaleWhitelist& v2);
    friend ostream& operator<<(ostream& os, const CAddToPublicSaleWhitelist& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CAddToPublicSaleWhitelist& ad);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAddToPublicSaleWhitelist::CAddToPublicSaleWhitelist(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAddToPublicSaleWhitelist::CAddToPublicSaleWhitelist(const CAddToPublicSaleWhitelist& ad) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ad);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAddToPublicSaleWhitelist::~CAddToPublicSaleWhitelist(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAddToPublicSaleWhitelist::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAddToPublicSaleWhitelist::initialize(void) {
    CTransaction::initialize();

    _bidder_addresses.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAddToPublicSaleWhitelist::duplicate(const CAddToPublicSaleWhitelist& ad) {
    clear();
    CTransaction::duplicate(ad);

    _bidder_addresses = ad._bidder_addresses;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CAddToPublicSaleWhitelist& CAddToPublicSaleWhitelist::operator=(const CAddToPublicSaleWhitelist& ad) {
    duplicate(ad);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CAddToPublicSaleWhitelist::operator==(const CAddToPublicSaleWhitelist& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CAddToPublicSaleWhitelist& v1, const CAddToPublicSaleWhitelist& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CAddToPublicSaleWhitelist> CAddToPublicSaleWhitelistArray;
extern CArchive& operator>>(CArchive& archive, CAddToPublicSaleWhitelistArray& array);
extern CArchive& operator<<(CArchive& archive, const CAddToPublicSaleWhitelistArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

