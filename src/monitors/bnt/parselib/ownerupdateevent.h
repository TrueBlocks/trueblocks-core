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
class COwnerUpdateEvent : public CLogEntry {
public:
    address_t _prevOwner;
    address_t _newOwner;

public:
    COwnerUpdateEvent(void);
    COwnerUpdateEvent(const COwnerUpdateEvent& ow);
    virtual ~COwnerUpdateEvent(void);
    COwnerUpdateEvent& operator=(const COwnerUpdateEvent& ow);

    DECLARE_NODE(COwnerUpdateEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const COwnerUpdateEvent& item) const;
    bool operator!=(const COwnerUpdateEvent& item) const { return !operator==(item); }
    friend bool operator<(const COwnerUpdateEvent& v1, const COwnerUpdateEvent& v2);
    friend ostream& operator<<(ostream& os, const COwnerUpdateEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const COwnerUpdateEvent& ow);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline COwnerUpdateEvent::COwnerUpdateEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline COwnerUpdateEvent::COwnerUpdateEvent(const COwnerUpdateEvent& ow) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ow);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline COwnerUpdateEvent::~COwnerUpdateEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COwnerUpdateEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COwnerUpdateEvent::initialize(void) {
    CLogEntry::initialize();

    _prevOwner = "";
    _newOwner = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COwnerUpdateEvent::duplicate(const COwnerUpdateEvent& ow) {
    clear();
    CLogEntry::duplicate(ow);

    _prevOwner = ow._prevOwner;
    _newOwner = ow._newOwner;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline COwnerUpdateEvent& COwnerUpdateEvent::operator=(const COwnerUpdateEvent& ow) {
    duplicate(ow);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool COwnerUpdateEvent::operator==(const COwnerUpdateEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const COwnerUpdateEvent& v1, const COwnerUpdateEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<COwnerUpdateEvent> COwnerUpdateEventArray;
extern CArchive& operator>>(CArchive& archive, COwnerUpdateEventArray& array);
extern CArchive& operator<<(CArchive& archive, const COwnerUpdateEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

