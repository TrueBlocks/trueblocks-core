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
class CCancelOrder : public CTransaction {
public:
    address_t tokenGet;
    biguint_t amountGet;
    address_t tokenGive;
    biguint_t amountGive;
    biguint_t expires;
    biguint_t nonce;
    uint32_t v;
    string_q r;
    string_q s;

public:
    CCancelOrder(void);
    CCancelOrder(const CCancelOrder& ca);
    virtual ~CCancelOrder(void);
    CCancelOrder& operator=(const CCancelOrder& ca);

    DECLARE_NODE(CCancelOrder);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CCancelOrder& item) const;
    bool operator!=(const CCancelOrder& item) const { return !operator==(item); }
    friend bool operator<(const CCancelOrder& v1, const CCancelOrder& v2);
    friend ostream& operator<<(ostream& os, const CCancelOrder& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CCancelOrder& ca);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CCancelOrder::CCancelOrder(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CCancelOrder::CCancelOrder(const CCancelOrder& ca) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ca);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CCancelOrder::~CCancelOrder(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCancelOrder::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCancelOrder::initialize(void) {
    CTransaction::initialize();

    tokenGet = "";
    amountGet = 0;
    tokenGive = "";
    amountGive = 0;
    expires = 0;
    nonce = 0;
    v = 0;
    r = "";
    s = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCancelOrder::duplicate(const CCancelOrder& ca) {
    clear();
    CTransaction::duplicate(ca);

    tokenGet = ca.tokenGet;
    amountGet = ca.amountGet;
    tokenGive = ca.tokenGive;
    amountGive = ca.amountGive;
    expires = ca.expires;
    nonce = ca.nonce;
    v = ca.v;
    r = ca.r;
    s = ca.s;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CCancelOrder& CCancelOrder::operator=(const CCancelOrder& ca) {
    duplicate(ca);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CCancelOrder::operator==(const CCancelOrder& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CCancelOrder& v1, const CCancelOrder& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CCancelOrder> CCancelOrderArray;
extern CArchive& operator>>(CArchive& archive, CCancelOrderArray& array);
extern CArchive& operator<<(CArchive& archive, const CCancelOrderArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

