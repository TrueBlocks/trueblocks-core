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
class CMembershipChangedEvent : public CLogEntry {
public:
    address_t member;

public:
    CMembershipChangedEvent(void);
    CMembershipChangedEvent(const CMembershipChangedEvent& me);
    virtual ~CMembershipChangedEvent(void);
    CMembershipChangedEvent& operator=(const CMembershipChangedEvent& me);

    DECLARE_NODE(CMembershipChangedEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CMembershipChangedEvent& item) const;
    bool operator!=(const CMembershipChangedEvent& item) const { return !operator==(item); }
    friend bool operator<(const CMembershipChangedEvent& v1, const CMembershipChangedEvent& v2);
    friend ostream& operator<<(ostream& os, const CMembershipChangedEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CMembershipChangedEvent& me);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CMembershipChangedEvent::CMembershipChangedEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CMembershipChangedEvent::CMembershipChangedEvent(const CMembershipChangedEvent& me) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(me);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CMembershipChangedEvent::~CMembershipChangedEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMembershipChangedEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMembershipChangedEvent::initialize(void) {
    CLogEntry::initialize();

    member = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMembershipChangedEvent::duplicate(const CMembershipChangedEvent& me) {
    clear();
    CLogEntry::duplicate(me);

    member = me.member;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CMembershipChangedEvent& CMembershipChangedEvent::operator=(const CMembershipChangedEvent& me) {
    duplicate(me);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CMembershipChangedEvent::operator==(const CMembershipChangedEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CMembershipChangedEvent& v1, const CMembershipChangedEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CMembershipChangedEvent> CMembershipChangedEventArray;
extern CArchive& operator>>(CArchive& archive, CMembershipChangedEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CMembershipChangedEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

