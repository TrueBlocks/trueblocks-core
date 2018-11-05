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
class CRegistrationStatusChangedEvent : public CLogEntry {
public:
    address_t target;
    bool isRegistered;

public:
    CRegistrationStatusChangedEvent(void);
    CRegistrationStatusChangedEvent(const CRegistrationStatusChangedEvent& re);
    virtual ~CRegistrationStatusChangedEvent(void);
    CRegistrationStatusChangedEvent& operator=(const CRegistrationStatusChangedEvent& re);

    DECLARE_NODE(CRegistrationStatusChangedEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CRegistrationStatusChangedEvent& item) const;
    bool operator!=(const CRegistrationStatusChangedEvent& item) const { return !operator==(item); }
    friend bool operator<(const CRegistrationStatusChangedEvent& v1, const CRegistrationStatusChangedEvent& v2);
    friend ostream& operator<<(ostream& os, const CRegistrationStatusChangedEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CRegistrationStatusChangedEvent& re);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CRegistrationStatusChangedEvent::CRegistrationStatusChangedEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CRegistrationStatusChangedEvent::CRegistrationStatusChangedEvent(const CRegistrationStatusChangedEvent& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CRegistrationStatusChangedEvent::~CRegistrationStatusChangedEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRegistrationStatusChangedEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRegistrationStatusChangedEvent::initialize(void) {
    CLogEntry::initialize();

    target = "";
    isRegistered = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRegistrationStatusChangedEvent::duplicate(const CRegistrationStatusChangedEvent& re) {
    clear();
    CLogEntry::duplicate(re);

    target = re.target;
    isRegistered = re.isRegistered;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CRegistrationStatusChangedEvent& CRegistrationStatusChangedEvent::operator=(const CRegistrationStatusChangedEvent& re) {
    duplicate(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CRegistrationStatusChangedEvent::operator==(const CRegistrationStatusChangedEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CRegistrationStatusChangedEvent& v1, const CRegistrationStatusChangedEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CRegistrationStatusChangedEvent> CRegistrationStatusChangedEventArray;
extern CArchive& operator>>(CArchive& archive, CRegistrationStatusChangedEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CRegistrationStatusChangedEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

