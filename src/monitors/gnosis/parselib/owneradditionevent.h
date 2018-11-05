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
class COwnerAdditionEvent : public CLogEntry {
public:
    address_t owner;

public:
    COwnerAdditionEvent(void);
    COwnerAdditionEvent(const COwnerAdditionEvent& ow);
    virtual ~COwnerAdditionEvent(void);
    COwnerAdditionEvent& operator=(const COwnerAdditionEvent& ow);

    DECLARE_NODE(COwnerAdditionEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const COwnerAdditionEvent& item) const;
    bool operator!=(const COwnerAdditionEvent& item) const { return !operator==(item); }
    friend bool operator<(const COwnerAdditionEvent& v1, const COwnerAdditionEvent& v2);
    friend ostream& operator<<(ostream& os, const COwnerAdditionEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const COwnerAdditionEvent& ow);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline COwnerAdditionEvent::COwnerAdditionEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline COwnerAdditionEvent::COwnerAdditionEvent(const COwnerAdditionEvent& ow) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ow);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline COwnerAdditionEvent::~COwnerAdditionEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COwnerAdditionEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COwnerAdditionEvent::initialize(void) {
    CLogEntry::initialize();

    owner = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COwnerAdditionEvent::duplicate(const COwnerAdditionEvent& ow) {
    clear();
    CLogEntry::duplicate(ow);

    owner = ow.owner;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline COwnerAdditionEvent& COwnerAdditionEvent::operator=(const COwnerAdditionEvent& ow) {
    duplicate(ow);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool COwnerAdditionEvent::operator==(const COwnerAdditionEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const COwnerAdditionEvent& v1, const COwnerAdditionEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<COwnerAdditionEvent> COwnerAdditionEventArray;
extern CArchive& operator>>(CArchive& archive, COwnerAdditionEventArray& array);
extern CArchive& operator<<(CArchive& archive, const COwnerAdditionEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

