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
class CRemovedFromPublicSaleWhitelistEvent : public CLogEntry {
public:
    address_t who;

public:
    CRemovedFromPublicSaleWhitelistEvent(void);
    CRemovedFromPublicSaleWhitelistEvent(const CRemovedFromPublicSaleWhitelistEvent& re);
    virtual ~CRemovedFromPublicSaleWhitelistEvent(void);
    CRemovedFromPublicSaleWhitelistEvent& operator=(const CRemovedFromPublicSaleWhitelistEvent& re);

    DECLARE_NODE(CRemovedFromPublicSaleWhitelistEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CRemovedFromPublicSaleWhitelistEvent& item) const;
    bool operator!=(const CRemovedFromPublicSaleWhitelistEvent& item) const { return !operator==(item); }
    friend bool operator<(const CRemovedFromPublicSaleWhitelistEvent& v1, const CRemovedFromPublicSaleWhitelistEvent& v2);
    friend ostream& operator<<(ostream& os, const CRemovedFromPublicSaleWhitelistEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CRemovedFromPublicSaleWhitelistEvent& re);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CRemovedFromPublicSaleWhitelistEvent::CRemovedFromPublicSaleWhitelistEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CRemovedFromPublicSaleWhitelistEvent::CRemovedFromPublicSaleWhitelistEvent(const CRemovedFromPublicSaleWhitelistEvent& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CRemovedFromPublicSaleWhitelistEvent::~CRemovedFromPublicSaleWhitelistEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRemovedFromPublicSaleWhitelistEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRemovedFromPublicSaleWhitelistEvent::initialize(void) {
    CLogEntry::initialize();

    who = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRemovedFromPublicSaleWhitelistEvent::duplicate(const CRemovedFromPublicSaleWhitelistEvent& re) {
    clear();
    CLogEntry::duplicate(re);

    who = re.who;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CRemovedFromPublicSaleWhitelistEvent& CRemovedFromPublicSaleWhitelistEvent::operator=(const CRemovedFromPublicSaleWhitelistEvent& re) {
    duplicate(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CRemovedFromPublicSaleWhitelistEvent::operator==(const CRemovedFromPublicSaleWhitelistEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CRemovedFromPublicSaleWhitelistEvent& v1, const CRemovedFromPublicSaleWhitelistEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CRemovedFromPublicSaleWhitelistEvent> CRemovedFromPublicSaleWhitelistEventArray;
extern CArchive& operator>>(CArchive& archive, CRemovedFromPublicSaleWhitelistEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CRemovedFromPublicSaleWhitelistEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

