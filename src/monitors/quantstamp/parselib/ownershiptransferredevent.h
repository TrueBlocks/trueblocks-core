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
class COwnershipTransferredEvent : public CLogEntry {
public:
    address_t previousOwner;
    address_t newOwner;

public:
    COwnershipTransferredEvent(void);
    COwnershipTransferredEvent(const COwnershipTransferredEvent& ow);
    virtual ~COwnershipTransferredEvent(void);
    COwnershipTransferredEvent& operator=(const COwnershipTransferredEvent& ow);

    DECLARE_NODE(COwnershipTransferredEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const COwnershipTransferredEvent& item) const;
    bool operator!=(const COwnershipTransferredEvent& item) const { return !operator==(item); }
    friend bool operator<(const COwnershipTransferredEvent& v1, const COwnershipTransferredEvent& v2);
    friend ostream& operator<<(ostream& os, const COwnershipTransferredEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const COwnershipTransferredEvent& ow);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline COwnershipTransferredEvent::COwnershipTransferredEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline COwnershipTransferredEvent::COwnershipTransferredEvent(const COwnershipTransferredEvent& ow) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ow);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline COwnershipTransferredEvent::~COwnershipTransferredEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COwnershipTransferredEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COwnershipTransferredEvent::initialize(void) {
    CLogEntry::initialize();

    previousOwner = "";
    newOwner = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COwnershipTransferredEvent::duplicate(const COwnershipTransferredEvent& ow) {
    clear();
    CLogEntry::duplicate(ow);

    previousOwner = ow.previousOwner;
    newOwner = ow.newOwner;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline COwnershipTransferredEvent& COwnershipTransferredEvent::operator=(const COwnershipTransferredEvent& ow) {
    duplicate(ow);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool COwnershipTransferredEvent::operator==(const COwnershipTransferredEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const COwnershipTransferredEvent& v1, const COwnershipTransferredEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<COwnershipTransferredEvent> COwnershipTransferredEventArray;
extern CArchive& operator>>(CArchive& archive, COwnershipTransferredEventArray& array);
extern CArchive& operator<<(CArchive& archive, const COwnershipTransferredEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

