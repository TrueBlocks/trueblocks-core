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
class CNewOwnerEvent : public CLogEntry {
public:
    address_t old;
    address_t current;

public:
    CNewOwnerEvent(void);
    CNewOwnerEvent(const CNewOwnerEvent& ne);
    virtual ~CNewOwnerEvent(void);
    CNewOwnerEvent& operator=(const CNewOwnerEvent& ne);

    DECLARE_NODE(CNewOwnerEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CNewOwnerEvent& item) const;
    bool operator!=(const CNewOwnerEvent& item) const { return !operator==(item); }
    friend bool operator<(const CNewOwnerEvent& v1, const CNewOwnerEvent& v2);
    friend ostream& operator<<(ostream& os, const CNewOwnerEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CNewOwnerEvent& ne);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CNewOwnerEvent::CNewOwnerEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CNewOwnerEvent::CNewOwnerEvent(const CNewOwnerEvent& ne) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ne);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CNewOwnerEvent::~CNewOwnerEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewOwnerEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewOwnerEvent::initialize(void) {
    CLogEntry::initialize();

    old = "";
    current = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewOwnerEvent::duplicate(const CNewOwnerEvent& ne) {
    clear();
    CLogEntry::duplicate(ne);

    old = ne.old;
    current = ne.current;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CNewOwnerEvent& CNewOwnerEvent::operator=(const CNewOwnerEvent& ne) {
    duplicate(ne);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CNewOwnerEvent::operator==(const CNewOwnerEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CNewOwnerEvent& v1, const CNewOwnerEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CNewOwnerEvent> CNewOwnerEventArray;
extern CArchive& operator>>(CArchive& archive, CNewOwnerEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CNewOwnerEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

