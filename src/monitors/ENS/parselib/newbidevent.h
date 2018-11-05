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
class CNewBidEvent : public CLogEntry {
public:
    string_q hash;
    address_t bidder;
    biguint_t deposit;

public:
    CNewBidEvent(void);
    CNewBidEvent(const CNewBidEvent& ne);
    virtual ~CNewBidEvent(void);
    CNewBidEvent& operator=(const CNewBidEvent& ne);

    DECLARE_NODE(CNewBidEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CNewBidEvent& item) const;
    bool operator!=(const CNewBidEvent& item) const { return !operator==(item); }
    friend bool operator<(const CNewBidEvent& v1, const CNewBidEvent& v2);
    friend ostream& operator<<(ostream& os, const CNewBidEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CNewBidEvent& ne);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CNewBidEvent::CNewBidEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CNewBidEvent::CNewBidEvent(const CNewBidEvent& ne) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ne);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CNewBidEvent::~CNewBidEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewBidEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewBidEvent::initialize(void) {
    CLogEntry::initialize();

    hash = "";
    bidder = "";
    deposit = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewBidEvent::duplicate(const CNewBidEvent& ne) {
    clear();
    CLogEntry::duplicate(ne);

    hash = ne.hash;
    bidder = ne.bidder;
    deposit = ne.deposit;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CNewBidEvent& CNewBidEvent::operator=(const CNewBidEvent& ne) {
    duplicate(ne);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CNewBidEvent::operator==(const CNewBidEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CNewBidEvent& v1, const CNewBidEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CNewBidEvent> CNewBidEventArray;
extern CArchive& operator>>(CArchive& archive, CNewBidEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CNewBidEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

