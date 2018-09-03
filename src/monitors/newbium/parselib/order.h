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
class COrder : public CTransaction {
public:
    address_t tokenGet;
    biguint_t amountGet;
    address_t tokenGive;
    biguint_t amountGive;
    biguint_t expires;
    biguint_t nonce;

public:
    COrder(void);
    COrder(const COrder& ord);
    virtual ~COrder(void);
    COrder& operator=(const COrder& ord);

    DECLARE_NODE(COrder);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const COrder& item) const;
    bool operator!=(const COrder& item) const { return !operator==(item); }
    friend bool operator<(const COrder& v1, const COrder& v2);
    friend ostream& operator<<(ostream& os, const COrder& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const COrder& ord);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline COrder::COrder(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline COrder::COrder(const COrder& ord) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ord);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline COrder::~COrder(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COrder::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COrder::initialize(void) {
    CTransaction::initialize();

    tokenGet = "";
    amountGet = 0;
    tokenGive = "";
    amountGive = 0;
    expires = 0;
    nonce = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COrder::duplicate(const COrder& ord) {
    clear();
    CTransaction::duplicate(ord);

    tokenGet = ord.tokenGet;
    amountGet = ord.amountGet;
    tokenGive = ord.tokenGive;
    amountGive = ord.amountGive;
    expires = ord.expires;
    nonce = ord.nonce;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline COrder& COrder::operator=(const COrder& ord) {
    duplicate(ord);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool COrder::operator==(const COrder& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const COrder& v1, const COrder& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<COrder> COrderArray;
extern CArchive& operator>>(CArchive& archive, COrderArray& array);
extern CArchive& operator<<(CArchive& archive, const COrderArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

