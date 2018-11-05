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
class CEmergencyStopActivatedEvent : public CLogEntry {
public:
    CEmergencyStopActivatedEvent(void);
    CEmergencyStopActivatedEvent(const CEmergencyStopActivatedEvent& em);
    virtual ~CEmergencyStopActivatedEvent(void);
    CEmergencyStopActivatedEvent& operator=(const CEmergencyStopActivatedEvent& em);

    DECLARE_NODE(CEmergencyStopActivatedEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CEmergencyStopActivatedEvent& item) const;
    bool operator!=(const CEmergencyStopActivatedEvent& item) const { return !operator==(item); }
    friend bool operator<(const CEmergencyStopActivatedEvent& v1, const CEmergencyStopActivatedEvent& v2);
    friend ostream& operator<<(ostream& os, const CEmergencyStopActivatedEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CEmergencyStopActivatedEvent& em);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CEmergencyStopActivatedEvent::CEmergencyStopActivatedEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CEmergencyStopActivatedEvent::CEmergencyStopActivatedEvent(const CEmergencyStopActivatedEvent& em) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(em);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CEmergencyStopActivatedEvent::~CEmergencyStopActivatedEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CEmergencyStopActivatedEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CEmergencyStopActivatedEvent::initialize(void) {
    CLogEntry::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CEmergencyStopActivatedEvent::duplicate(const CEmergencyStopActivatedEvent& em) {
    clear();
    CLogEntry::duplicate(em);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CEmergencyStopActivatedEvent& CEmergencyStopActivatedEvent::operator=(const CEmergencyStopActivatedEvent& em) {
    duplicate(em);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CEmergencyStopActivatedEvent::operator==(const CEmergencyStopActivatedEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CEmergencyStopActivatedEvent& v1, const CEmergencyStopActivatedEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CEmergencyStopActivatedEvent> CEmergencyStopActivatedEventArray;
extern CArchive& operator>>(CArchive& archive, CEmergencyStopActivatedEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CEmergencyStopActivatedEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

