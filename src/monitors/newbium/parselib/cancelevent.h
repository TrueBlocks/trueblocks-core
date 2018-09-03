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
class CCancelEvent : public CLogEntry {
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

public:
    CCancelEvent(void);
    CCancelEvent(const CCancelEvent& ca);
    virtual ~CCancelEvent(void);
    CCancelEvent& operator=(const CCancelEvent& ca);

    DECLARE_NODE(CCancelEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CCancelEvent& item) const;
    bool operator!=(const CCancelEvent& item) const { return !operator==(item); }
    friend bool operator<(const CCancelEvent& v1, const CCancelEvent& v2);
    friend ostream& operator<<(ostream& os, const CCancelEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CCancelEvent& ca);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CCancelEvent::CCancelEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CCancelEvent::CCancelEvent(const CCancelEvent& ca) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ca);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CCancelEvent::~CCancelEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCancelEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCancelEvent::initialize(void) {
    CLogEntry::initialize();

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

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCancelEvent::duplicate(const CCancelEvent& ca) {
    clear();
    CLogEntry::duplicate(ca);

    tokenGet = ca.tokenGet;
    amountGet = ca.amountGet;
    tokenGive = ca.tokenGive;
    amountGive = ca.amountGive;
    expires = ca.expires;
    nonce = ca.nonce;
    user = ca.user;
    v = ca.v;
    r = ca.r;
    s = ca.s;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CCancelEvent& CCancelEvent::operator=(const CCancelEvent& ca) {
    duplicate(ca);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CCancelEvent::operator==(const CCancelEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CCancelEvent& v1, const CCancelEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CCancelEvent> CCancelEventArray;
extern CArchive& operator>>(CArchive& archive, CCancelEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CCancelEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

