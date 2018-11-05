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
class CCancelRequestEvent : public CLogEntry {
public:
    biguint_t _key;
    address_t _sender;
    biguint_t _deposit;
    biguint_t _amount;

public:
    CCancelRequestEvent(void);
    CCancelRequestEvent(const CCancelRequestEvent& ca);
    virtual ~CCancelRequestEvent(void);
    CCancelRequestEvent& operator=(const CCancelRequestEvent& ca);

    DECLARE_NODE(CCancelRequestEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CCancelRequestEvent& item) const;
    bool operator!=(const CCancelRequestEvent& item) const { return !operator==(item); }
    friend bool operator<(const CCancelRequestEvent& v1, const CCancelRequestEvent& v2);
    friend ostream& operator<<(ostream& os, const CCancelRequestEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CCancelRequestEvent& ca);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CCancelRequestEvent::CCancelRequestEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CCancelRequestEvent::CCancelRequestEvent(const CCancelRequestEvent& ca) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ca);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CCancelRequestEvent::~CCancelRequestEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCancelRequestEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCancelRequestEvent::initialize(void) {
    CLogEntry::initialize();

    _key = 0;
    _sender = "";
    _deposit = 0;
    _amount = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCancelRequestEvent::duplicate(const CCancelRequestEvent& ca) {
    clear();
    CLogEntry::duplicate(ca);

    _key = ca._key;
    _sender = ca._sender;
    _deposit = ca._deposit;
    _amount = ca._amount;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CCancelRequestEvent& CCancelRequestEvent::operator=(const CCancelRequestEvent& ca) {
    duplicate(ca);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CCancelRequestEvent::operator==(const CCancelRequestEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CCancelRequestEvent& v1, const CCancelRequestEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CCancelRequestEvent> CCancelRequestEventArray;
extern CArchive& operator>>(CArchive& archive, CCancelRequestEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CCancelRequestEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

