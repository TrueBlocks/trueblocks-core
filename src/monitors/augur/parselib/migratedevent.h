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
class CMigratedEvent : public CLogEntry {
public:
    address_t holder;
    biguint_t amount;

public:
    CMigratedEvent(void);
    CMigratedEvent(const CMigratedEvent& mi);
    virtual ~CMigratedEvent(void);
    CMigratedEvent& operator=(const CMigratedEvent& mi);

    DECLARE_NODE(CMigratedEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CMigratedEvent& item) const;
    bool operator!=(const CMigratedEvent& item) const { return !operator==(item); }
    friend bool operator<(const CMigratedEvent& v1, const CMigratedEvent& v2);
    friend ostream& operator<<(ostream& os, const CMigratedEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CMigratedEvent& mi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CMigratedEvent::CMigratedEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CMigratedEvent::CMigratedEvent(const CMigratedEvent& mi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(mi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CMigratedEvent::~CMigratedEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMigratedEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMigratedEvent::initialize(void) {
    CLogEntry::initialize();

    holder = "";
    amount = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMigratedEvent::duplicate(const CMigratedEvent& mi) {
    clear();
    CLogEntry::duplicate(mi);

    holder = mi.holder;
    amount = mi.amount;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CMigratedEvent& CMigratedEvent::operator=(const CMigratedEvent& mi) {
    duplicate(mi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CMigratedEvent::operator==(const CMigratedEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CMigratedEvent& v1, const CMigratedEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CMigratedEvent> CMigratedEventArray;
extern CArchive& operator>>(CArchive& archive, CMigratedEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CMigratedEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

