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
class CNewHandleEvent : public CLogEntry {
public:
    string_q _base;
    string_q _handle;
    address_t _address;

public:
    CNewHandleEvent(void);
    CNewHandleEvent(const CNewHandleEvent& ne);
    virtual ~CNewHandleEvent(void);
    CNewHandleEvent& operator=(const CNewHandleEvent& ne);

    DECLARE_NODE(CNewHandleEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CNewHandleEvent& item) const;
    bool operator!=(const CNewHandleEvent& item) const { return !operator==(item); }
    friend bool operator<(const CNewHandleEvent& v1, const CNewHandleEvent& v2);
    friend ostream& operator<<(ostream& os, const CNewHandleEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CNewHandleEvent& ne);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CNewHandleEvent::CNewHandleEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CNewHandleEvent::CNewHandleEvent(const CNewHandleEvent& ne) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ne);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CNewHandleEvent::~CNewHandleEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewHandleEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewHandleEvent::initialize(void) {
    CLogEntry::initialize();

    _base = "";
    _handle = "";
    _address = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewHandleEvent::duplicate(const CNewHandleEvent& ne) {
    clear();
    CLogEntry::duplicate(ne);

    _base = ne._base;
    _handle = ne._handle;
    _address = ne._address;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CNewHandleEvent& CNewHandleEvent::operator=(const CNewHandleEvent& ne) {
    duplicate(ne);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CNewHandleEvent::operator==(const CNewHandleEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CNewHandleEvent& v1, const CNewHandleEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CNewHandleEvent> CNewHandleEventArray;
extern CArchive& operator>>(CArchive& archive, CNewHandleEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CNewHandleEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

