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
class CNewBaseEvent : public CLogEntry {
public:
    string_q _base;
    address_t _address;

public:
    CNewBaseEvent(void);
    CNewBaseEvent(const CNewBaseEvent& ne);
    virtual ~CNewBaseEvent(void);
    CNewBaseEvent& operator=(const CNewBaseEvent& ne);

    DECLARE_NODE(CNewBaseEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CNewBaseEvent& item) const;
    bool operator!=(const CNewBaseEvent& item) const { return !operator==(item); }
    friend bool operator<(const CNewBaseEvent& v1, const CNewBaseEvent& v2);
    friend ostream& operator<<(ostream& os, const CNewBaseEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CNewBaseEvent& ne);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CNewBaseEvent::CNewBaseEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CNewBaseEvent::CNewBaseEvent(const CNewBaseEvent& ne) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ne);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CNewBaseEvent::~CNewBaseEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewBaseEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewBaseEvent::initialize(void) {
    CLogEntry::initialize();

    _base = "";
    _address = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewBaseEvent::duplicate(const CNewBaseEvent& ne) {
    clear();
    CLogEntry::duplicate(ne);

    _base = ne._base;
    _address = ne._address;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CNewBaseEvent& CNewBaseEvent::operator=(const CNewBaseEvent& ne) {
    duplicate(ne);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CNewBaseEvent::operator==(const CNewBaseEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CNewBaseEvent& v1, const CNewBaseEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CNewBaseEvent> CNewBaseEventArray;
extern CArchive& operator>>(CArchive& archive, CNewBaseEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CNewBaseEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

