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
class CTrade : public CTransaction {
public:
    address_t tokenGet;
    biguint_t amountGet;
    address_t tokenGive;
    biguint_t amountGive;
    biguint_t expires;
    biguint_t nonce;
    address_t user;
    uint32_t v;
    string_q r;
    string_q s;
    biguint_t amount;

public:
    CTrade(void);
    CTrade(const CTrade& tr);
    virtual ~CTrade(void);
    CTrade& operator=(const CTrade& tr);

    DECLARE_NODE(CTrade);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CTrade& item) const;
    bool operator!=(const CTrade& item) const { return !operator==(item); }
    friend bool operator<(const CTrade& v1, const CTrade& v2);
    friend ostream& operator<<(ostream& os, const CTrade& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CTrade& tr);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTrade::CTrade(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTrade::CTrade(const CTrade& tr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(tr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTrade::~CTrade(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTrade::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTrade::initialize(void) {
    CTransaction::initialize();

    tokenGet = "";
    amountGet = 0;
    tokenGive = "";
    amountGive = 0;
    expires = 0;
    nonce = 0;
    user = "";
    v = 0;
    r = "";
    s = "";
    amount = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTrade::duplicate(const CTrade& tr) {
    clear();
    CTransaction::duplicate(tr);

    tokenGet = tr.tokenGet;
    amountGet = tr.amountGet;
    tokenGive = tr.tokenGive;
    amountGive = tr.amountGive;
    expires = tr.expires;
    nonce = tr.nonce;
    user = tr.user;
    v = tr.v;
    r = tr.r;
    s = tr.s;
    amount = tr.amount;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CTrade& CTrade::operator=(const CTrade& tr) {
    duplicate(tr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CTrade::operator==(const CTrade& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CTrade& v1, const CTrade& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CTrade> CTradeArray;
extern CArchive& operator>>(CArchive& archive, CTradeArray& array);
extern CArchive& operator<<(CArchive& archive, const CTradeArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

