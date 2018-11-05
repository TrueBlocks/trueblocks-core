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
class CRequestedEvent : public CLogEntry {
public:
    address_t who;

public:
    CRequestedEvent(void);
    CRequestedEvent(const CRequestedEvent& re);
    virtual ~CRequestedEvent(void);
    CRequestedEvent& operator=(const CRequestedEvent& re);

    DECLARE_NODE(CRequestedEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CRequestedEvent& item) const;
    bool operator!=(const CRequestedEvent& item) const { return !operator==(item); }
    friend bool operator<(const CRequestedEvent& v1, const CRequestedEvent& v2);
    friend ostream& operator<<(ostream& os, const CRequestedEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CRequestedEvent& re);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CRequestedEvent::CRequestedEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CRequestedEvent::CRequestedEvent(const CRequestedEvent& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CRequestedEvent::~CRequestedEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRequestedEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRequestedEvent::initialize(void) {
    CLogEntry::initialize();

    who = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRequestedEvent::duplicate(const CRequestedEvent& re) {
    clear();
    CLogEntry::duplicate(re);

    who = re.who;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CRequestedEvent& CRequestedEvent::operator=(const CRequestedEvent& re) {
    duplicate(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CRequestedEvent::operator==(const CRequestedEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CRequestedEvent& v1, const CRequestedEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CRequestedEvent> CRequestedEventArray;
extern CArchive& operator>>(CArchive& archive, CRequestedEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CRequestedEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

