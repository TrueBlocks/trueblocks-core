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
class CAcceptRequestEvent : public CLogEntry {
public:
    biguint_t _key;
    biguint_t _etherAmount;
    biguint_t _tokenAmount;
    biguint_t _dollarPrice;
    address_t _acceptor;

public:
    CAcceptRequestEvent(void);
    CAcceptRequestEvent(const CAcceptRequestEvent& ac);
    virtual ~CAcceptRequestEvent(void);
    CAcceptRequestEvent& operator=(const CAcceptRequestEvent& ac);

    DECLARE_NODE(CAcceptRequestEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CAcceptRequestEvent& item) const;
    bool operator!=(const CAcceptRequestEvent& item) const { return !operator==(item); }
    friend bool operator<(const CAcceptRequestEvent& v1, const CAcceptRequestEvent& v2);
    friend ostream& operator<<(ostream& os, const CAcceptRequestEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CAcceptRequestEvent& ac);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAcceptRequestEvent::CAcceptRequestEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAcceptRequestEvent::CAcceptRequestEvent(const CAcceptRequestEvent& ac) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ac);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAcceptRequestEvent::~CAcceptRequestEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAcceptRequestEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAcceptRequestEvent::initialize(void) {
    CLogEntry::initialize();

    _key = 0;
    _etherAmount = 0;
    _tokenAmount = 0;
    _dollarPrice = 0;
    _acceptor = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAcceptRequestEvent::duplicate(const CAcceptRequestEvent& ac) {
    clear();
    CLogEntry::duplicate(ac);

    _key = ac._key;
    _etherAmount = ac._etherAmount;
    _tokenAmount = ac._tokenAmount;
    _dollarPrice = ac._dollarPrice;
    _acceptor = ac._acceptor;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CAcceptRequestEvent& CAcceptRequestEvent::operator=(const CAcceptRequestEvent& ac) {
    duplicate(ac);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CAcceptRequestEvent::operator==(const CAcceptRequestEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CAcceptRequestEvent& v1, const CAcceptRequestEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CAcceptRequestEvent> CAcceptRequestEventArray;
extern CArchive& operator>>(CArchive& archive, CAcceptRequestEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CAcceptRequestEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

