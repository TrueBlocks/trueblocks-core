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
class CPrivateSaleWhitelistedEvent : public CLogEntry {
public:
    address_t who;

public:
    CPrivateSaleWhitelistedEvent(void);
    CPrivateSaleWhitelistedEvent(const CPrivateSaleWhitelistedEvent& pr);
    virtual ~CPrivateSaleWhitelistedEvent(void);
    CPrivateSaleWhitelistedEvent& operator=(const CPrivateSaleWhitelistedEvent& pr);

    DECLARE_NODE(CPrivateSaleWhitelistedEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CPrivateSaleWhitelistedEvent& item) const;
    bool operator!=(const CPrivateSaleWhitelistedEvent& item) const { return !operator==(item); }
    friend bool operator<(const CPrivateSaleWhitelistedEvent& v1, const CPrivateSaleWhitelistedEvent& v2);
    friend ostream& operator<<(ostream& os, const CPrivateSaleWhitelistedEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CPrivateSaleWhitelistedEvent& pr);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CPrivateSaleWhitelistedEvent::CPrivateSaleWhitelistedEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPrivateSaleWhitelistedEvent::CPrivateSaleWhitelistedEvent(const CPrivateSaleWhitelistedEvent& pr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(pr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CPrivateSaleWhitelistedEvent::~CPrivateSaleWhitelistedEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPrivateSaleWhitelistedEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPrivateSaleWhitelistedEvent::initialize(void) {
    CLogEntry::initialize();

    who = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPrivateSaleWhitelistedEvent::duplicate(const CPrivateSaleWhitelistedEvent& pr) {
    clear();
    CLogEntry::duplicate(pr);

    who = pr.who;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CPrivateSaleWhitelistedEvent& CPrivateSaleWhitelistedEvent::operator=(const CPrivateSaleWhitelistedEvent& pr) {
    duplicate(pr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CPrivateSaleWhitelistedEvent::operator==(const CPrivateSaleWhitelistedEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CPrivateSaleWhitelistedEvent& v1, const CPrivateSaleWhitelistedEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CPrivateSaleWhitelistedEvent> CPrivateSaleWhitelistedEventArray;
extern CArchive& operator>>(CArchive& archive, CPrivateSaleWhitelistedEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CPrivateSaleWhitelistedEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

