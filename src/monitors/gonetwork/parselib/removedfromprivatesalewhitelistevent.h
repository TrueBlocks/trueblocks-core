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
class CRemovedFromPrivateSaleWhitelistEvent : public CLogEntry {
public:
    address_t who;

public:
    CRemovedFromPrivateSaleWhitelistEvent(void);
    CRemovedFromPrivateSaleWhitelistEvent(const CRemovedFromPrivateSaleWhitelistEvent& re);
    virtual ~CRemovedFromPrivateSaleWhitelistEvent(void);
    CRemovedFromPrivateSaleWhitelistEvent& operator=(const CRemovedFromPrivateSaleWhitelistEvent& re);

    DECLARE_NODE(CRemovedFromPrivateSaleWhitelistEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CRemovedFromPrivateSaleWhitelistEvent& item) const;
    bool operator!=(const CRemovedFromPrivateSaleWhitelistEvent& item) const { return !operator==(item); }
    friend bool operator<(const CRemovedFromPrivateSaleWhitelistEvent& v1, const CRemovedFromPrivateSaleWhitelistEvent& v2);
    friend ostream& operator<<(ostream& os, const CRemovedFromPrivateSaleWhitelistEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CRemovedFromPrivateSaleWhitelistEvent& re);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CRemovedFromPrivateSaleWhitelistEvent::CRemovedFromPrivateSaleWhitelistEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CRemovedFromPrivateSaleWhitelistEvent::CRemovedFromPrivateSaleWhitelistEvent(const CRemovedFromPrivateSaleWhitelistEvent& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CRemovedFromPrivateSaleWhitelistEvent::~CRemovedFromPrivateSaleWhitelistEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRemovedFromPrivateSaleWhitelistEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRemovedFromPrivateSaleWhitelistEvent::initialize(void) {
    CLogEntry::initialize();

    who = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRemovedFromPrivateSaleWhitelistEvent::duplicate(const CRemovedFromPrivateSaleWhitelistEvent& re) {
    clear();
    CLogEntry::duplicate(re);

    who = re.who;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CRemovedFromPrivateSaleWhitelistEvent& CRemovedFromPrivateSaleWhitelistEvent::operator=(const CRemovedFromPrivateSaleWhitelistEvent& re) {
    duplicate(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CRemovedFromPrivateSaleWhitelistEvent::operator==(const CRemovedFromPrivateSaleWhitelistEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CRemovedFromPrivateSaleWhitelistEvent& v1, const CRemovedFromPrivateSaleWhitelistEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CRemovedFromPrivateSaleWhitelistEvent> CRemovedFromPrivateSaleWhitelistEventArray;
extern CArchive& operator>>(CArchive& archive, CRemovedFromPrivateSaleWhitelistEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CRemovedFromPrivateSaleWhitelistEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

