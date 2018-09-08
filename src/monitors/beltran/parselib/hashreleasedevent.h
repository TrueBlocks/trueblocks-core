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
class CHashReleasedEvent : public CLogEntry {
public:
    string_q hash;
    biguint_t value;

public:
    CHashReleasedEvent(void);
    CHashReleasedEvent(const CHashReleasedEvent& ha);
    virtual ~CHashReleasedEvent(void);
    CHashReleasedEvent& operator=(const CHashReleasedEvent& ha);

    DECLARE_NODE(CHashReleasedEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CHashReleasedEvent& item) const;
    bool operator!=(const CHashReleasedEvent& item) const { return !operator==(item); }
    friend bool operator<(const CHashReleasedEvent& v1, const CHashReleasedEvent& v2);
    friend ostream& operator<<(ostream& os, const CHashReleasedEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CHashReleasedEvent& ha);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CHashReleasedEvent::CHashReleasedEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CHashReleasedEvent::CHashReleasedEvent(const CHashReleasedEvent& ha) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ha);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CHashReleasedEvent::~CHashReleasedEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CHashReleasedEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CHashReleasedEvent::initialize(void) {
    CLogEntry::initialize();

    hash = "";
    value = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CHashReleasedEvent::duplicate(const CHashReleasedEvent& ha) {
    clear();
    CLogEntry::duplicate(ha);

    hash = ha.hash;
    value = ha.value;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CHashReleasedEvent& CHashReleasedEvent::operator=(const CHashReleasedEvent& ha) {
    duplicate(ha);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CHashReleasedEvent::operator==(const CHashReleasedEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CHashReleasedEvent& v1, const CHashReleasedEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CHashReleasedEvent> CHashReleasedEventArray;
extern CArchive& operator>>(CArchive& archive, CHashReleasedEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CHashReleasedEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

