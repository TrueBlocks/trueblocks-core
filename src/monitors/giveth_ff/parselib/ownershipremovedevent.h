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
class COwnershipRemovedEvent : public CLogEntry {
public:
    COwnershipRemovedEvent(void);
    COwnershipRemovedEvent(const COwnershipRemovedEvent& ow);
    virtual ~COwnershipRemovedEvent(void);
    COwnershipRemovedEvent& operator=(const COwnershipRemovedEvent& ow);

    DECLARE_NODE(COwnershipRemovedEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const COwnershipRemovedEvent& item) const;
    bool operator!=(const COwnershipRemovedEvent& item) const { return !operator==(item); }
    friend bool operator<(const COwnershipRemovedEvent& v1, const COwnershipRemovedEvent& v2);
    friend ostream& operator<<(ostream& os, const COwnershipRemovedEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const COwnershipRemovedEvent& ow);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline COwnershipRemovedEvent::COwnershipRemovedEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline COwnershipRemovedEvent::COwnershipRemovedEvent(const COwnershipRemovedEvent& ow) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ow);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline COwnershipRemovedEvent::~COwnershipRemovedEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COwnershipRemovedEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COwnershipRemovedEvent::initialize(void) {
    CLogEntry::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COwnershipRemovedEvent::duplicate(const COwnershipRemovedEvent& ow) {
    clear();
    CLogEntry::duplicate(ow);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline COwnershipRemovedEvent& COwnershipRemovedEvent::operator=(const COwnershipRemovedEvent& ow) {
    duplicate(ow);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool COwnershipRemovedEvent::operator==(const COwnershipRemovedEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const COwnershipRemovedEvent& v1, const COwnershipRemovedEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<COwnershipRemovedEvent> COwnershipRemovedEventArray;
extern CArchive& operator>>(CArchive& archive, COwnershipRemovedEventArray& array);
extern CArchive& operator<<(CArchive& archive, const COwnershipRemovedEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

