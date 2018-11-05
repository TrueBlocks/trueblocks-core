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
class CMakeRequestEvent : public CLogEntry {
public:
    biguint_t _key;
    biguint_t _date;
    biguint_t _tokenAmount;
    biguint_t _dollarPrice;
    uint32_t _requestType;
    address_t _requester;

public:
    CMakeRequestEvent(void);
    CMakeRequestEvent(const CMakeRequestEvent& ma);
    virtual ~CMakeRequestEvent(void);
    CMakeRequestEvent& operator=(const CMakeRequestEvent& ma);

    DECLARE_NODE(CMakeRequestEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CMakeRequestEvent& item) const;
    bool operator!=(const CMakeRequestEvent& item) const { return !operator==(item); }
    friend bool operator<(const CMakeRequestEvent& v1, const CMakeRequestEvent& v2);
    friend ostream& operator<<(ostream& os, const CMakeRequestEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CMakeRequestEvent& ma);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CMakeRequestEvent::CMakeRequestEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CMakeRequestEvent::CMakeRequestEvent(const CMakeRequestEvent& ma) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ma);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CMakeRequestEvent::~CMakeRequestEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMakeRequestEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMakeRequestEvent::initialize(void) {
    CLogEntry::initialize();

    _key = 0;
    _date = 0;
    _tokenAmount = 0;
    _dollarPrice = 0;
    _requestType = 0;
    _requester = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMakeRequestEvent::duplicate(const CMakeRequestEvent& ma) {
    clear();
    CLogEntry::duplicate(ma);

    _key = ma._key;
    _date = ma._date;
    _tokenAmount = ma._tokenAmount;
    _dollarPrice = ma._dollarPrice;
    _requestType = ma._requestType;
    _requester = ma._requester;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CMakeRequestEvent& CMakeRequestEvent::operator=(const CMakeRequestEvent& ma) {
    duplicate(ma);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CMakeRequestEvent::operator==(const CMakeRequestEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CMakeRequestEvent& v1, const CMakeRequestEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CMakeRequestEvent> CMakeRequestEventArray;
extern CArchive& operator>>(CArchive& archive, CMakeRequestEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CMakeRequestEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

