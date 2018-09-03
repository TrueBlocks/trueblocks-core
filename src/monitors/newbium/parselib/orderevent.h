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
#include "logentry.h"

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class COrderEvent : public CLogEntry {
public:
    address_t tokenGet;
    biguint_t amountGet;
    address_t tokenGive;
    biguint_t amountGive;
    biguint_t expires;
    biguint_t nonce;
    address_t user;

public:
    COrderEvent(void);
    COrderEvent(const COrderEvent& ord);
    virtual ~COrderEvent(void);
    COrderEvent& operator=(const COrderEvent& ord);

    DECLARE_NODE(COrderEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const COrderEvent& item) const;
    bool operator!=(const COrderEvent& item) const { return !operator==(item); }
    friend bool operator<(const COrderEvent& v1, const COrderEvent& v2);
    friend ostream& operator<<(ostream& os, const COrderEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const COrderEvent& ord);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline COrderEvent::COrderEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline COrderEvent::COrderEvent(const COrderEvent& ord) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ord);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline COrderEvent::~COrderEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COrderEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COrderEvent::initialize(void) {
    CLogEntry::initialize();

    tokenGet = "";
    amountGet = 0;
    tokenGive = "";
    amountGive = 0;
    expires = 0;
    nonce = 0;
    user = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COrderEvent::duplicate(const COrderEvent& ord) {
    clear();
    CLogEntry::duplicate(ord);

    tokenGet = ord.tokenGet;
    amountGet = ord.amountGet;
    tokenGive = ord.tokenGive;
    amountGive = ord.amountGive;
    expires = ord.expires;
    nonce = ord.nonce;
    user = ord.user;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline COrderEvent& COrderEvent::operator=(const COrderEvent& ord) {
    duplicate(ord);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool COrderEvent::operator==(const COrderEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const COrderEvent& v1, const COrderEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<COrderEvent> COrderEventArray;
extern CArchive& operator>>(CArchive& archive, COrderEventArray& array);
extern CArchive& operator<<(CArchive& archive, const COrderEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

