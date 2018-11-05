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
class CHashInvalidatedEvent : public CLogEntry {
public:
    string_q hash;
    string_q name;
    biguint_t value;
    biguint_t registrationDate;

public:
    CHashInvalidatedEvent(void);
    CHashInvalidatedEvent(const CHashInvalidatedEvent& ha);
    virtual ~CHashInvalidatedEvent(void);
    CHashInvalidatedEvent& operator=(const CHashInvalidatedEvent& ha);

    DECLARE_NODE(CHashInvalidatedEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CHashInvalidatedEvent& item) const;
    bool operator!=(const CHashInvalidatedEvent& item) const { return !operator==(item); }
    friend bool operator<(const CHashInvalidatedEvent& v1, const CHashInvalidatedEvent& v2);
    friend ostream& operator<<(ostream& os, const CHashInvalidatedEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CHashInvalidatedEvent& ha);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CHashInvalidatedEvent::CHashInvalidatedEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CHashInvalidatedEvent::CHashInvalidatedEvent(const CHashInvalidatedEvent& ha) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ha);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CHashInvalidatedEvent::~CHashInvalidatedEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CHashInvalidatedEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CHashInvalidatedEvent::initialize(void) {
    CLogEntry::initialize();

    hash = "";
    name = "";
    value = 0;
    registrationDate = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CHashInvalidatedEvent::duplicate(const CHashInvalidatedEvent& ha) {
    clear();
    CLogEntry::duplicate(ha);

    hash = ha.hash;
    name = ha.name;
    value = ha.value;
    registrationDate = ha.registrationDate;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CHashInvalidatedEvent& CHashInvalidatedEvent::operator=(const CHashInvalidatedEvent& ha) {
    duplicate(ha);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CHashInvalidatedEvent::operator==(const CHashInvalidatedEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CHashInvalidatedEvent& v1, const CHashInvalidatedEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CHashInvalidatedEvent> CHashInvalidatedEventArray;
extern CArchive& operator>>(CArchive& archive, CHashInvalidatedEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CHashInvalidatedEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

