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
class CHashRegisteredEvent : public CLogEntry {
public:
    string_q hash;
    address_t owner;
    biguint_t value;
    biguint_t registrationDate;

public:
    CHashRegisteredEvent(void);
    CHashRegisteredEvent(const CHashRegisteredEvent& ha);
    virtual ~CHashRegisteredEvent(void);
    CHashRegisteredEvent& operator=(const CHashRegisteredEvent& ha);

    DECLARE_NODE(CHashRegisteredEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CHashRegisteredEvent& item) const;
    bool operator!=(const CHashRegisteredEvent& item) const { return !operator==(item); }
    friend bool operator<(const CHashRegisteredEvent& v1, const CHashRegisteredEvent& v2);
    friend ostream& operator<<(ostream& os, const CHashRegisteredEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CHashRegisteredEvent& ha);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CHashRegisteredEvent::CHashRegisteredEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CHashRegisteredEvent::CHashRegisteredEvent(const CHashRegisteredEvent& ha) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ha);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CHashRegisteredEvent::~CHashRegisteredEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CHashRegisteredEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CHashRegisteredEvent::initialize(void) {
    CLogEntry::initialize();

    hash = "";
    owner = "";
    value = 0;
    registrationDate = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CHashRegisteredEvent::duplicate(const CHashRegisteredEvent& ha) {
    clear();
    CLogEntry::duplicate(ha);

    hash = ha.hash;
    owner = ha.owner;
    value = ha.value;
    registrationDate = ha.registrationDate;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CHashRegisteredEvent& CHashRegisteredEvent::operator=(const CHashRegisteredEvent& ha) {
    duplicate(ha);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CHashRegisteredEvent::operator==(const CHashRegisteredEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CHashRegisteredEvent& v1, const CHashRegisteredEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CHashRegisteredEvent> CHashRegisteredEventArray;
extern CArchive& operator>>(CArchive& archive, CHashRegisteredEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CHashRegisteredEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

