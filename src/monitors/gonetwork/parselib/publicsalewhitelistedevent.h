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
class CPublicSaleWhitelistedEvent : public CLogEntry {
public:
    address_t who;

public:
    CPublicSaleWhitelistedEvent(void);
    CPublicSaleWhitelistedEvent(const CPublicSaleWhitelistedEvent& pu);
    virtual ~CPublicSaleWhitelistedEvent(void);
    CPublicSaleWhitelistedEvent& operator=(const CPublicSaleWhitelistedEvent& pu);

    DECLARE_NODE(CPublicSaleWhitelistedEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CPublicSaleWhitelistedEvent& item) const;
    bool operator!=(const CPublicSaleWhitelistedEvent& item) const { return !operator==(item); }
    friend bool operator<(const CPublicSaleWhitelistedEvent& v1, const CPublicSaleWhitelistedEvent& v2);
    friend ostream& operator<<(ostream& os, const CPublicSaleWhitelistedEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CPublicSaleWhitelistedEvent& pu);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CPublicSaleWhitelistedEvent::CPublicSaleWhitelistedEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPublicSaleWhitelistedEvent::CPublicSaleWhitelistedEvent(const CPublicSaleWhitelistedEvent& pu) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(pu);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CPublicSaleWhitelistedEvent::~CPublicSaleWhitelistedEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPublicSaleWhitelistedEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPublicSaleWhitelistedEvent::initialize(void) {
    CLogEntry::initialize();

    who = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPublicSaleWhitelistedEvent::duplicate(const CPublicSaleWhitelistedEvent& pu) {
    clear();
    CLogEntry::duplicate(pu);

    who = pu.who;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CPublicSaleWhitelistedEvent& CPublicSaleWhitelistedEvent::operator=(const CPublicSaleWhitelistedEvent& pu) {
    duplicate(pu);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CPublicSaleWhitelistedEvent::operator==(const CPublicSaleWhitelistedEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CPublicSaleWhitelistedEvent& v1, const CPublicSaleWhitelistedEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CPublicSaleWhitelistedEvent> CPublicSaleWhitelistedEventArray;
extern CArchive& operator>>(CArchive& archive, CPublicSaleWhitelistedEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CPublicSaleWhitelistedEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

