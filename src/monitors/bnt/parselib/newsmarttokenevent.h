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
class CNewSmartTokenEvent : public CLogEntry {
public:
    address_t _token;

public:
    CNewSmartTokenEvent(void);
    CNewSmartTokenEvent(const CNewSmartTokenEvent& ne);
    virtual ~CNewSmartTokenEvent(void);
    CNewSmartTokenEvent& operator=(const CNewSmartTokenEvent& ne);

    DECLARE_NODE(CNewSmartTokenEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CNewSmartTokenEvent& item) const;
    bool operator!=(const CNewSmartTokenEvent& item) const { return !operator==(item); }
    friend bool operator<(const CNewSmartTokenEvent& v1, const CNewSmartTokenEvent& v2);
    friend ostream& operator<<(ostream& os, const CNewSmartTokenEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CNewSmartTokenEvent& ne);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CNewSmartTokenEvent::CNewSmartTokenEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CNewSmartTokenEvent::CNewSmartTokenEvent(const CNewSmartTokenEvent& ne) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ne);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CNewSmartTokenEvent::~CNewSmartTokenEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewSmartTokenEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewSmartTokenEvent::initialize(void) {
    CLogEntry::initialize();

    _token = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewSmartTokenEvent::duplicate(const CNewSmartTokenEvent& ne) {
    clear();
    CLogEntry::duplicate(ne);

    _token = ne._token;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CNewSmartTokenEvent& CNewSmartTokenEvent::operator=(const CNewSmartTokenEvent& ne) {
    duplicate(ne);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CNewSmartTokenEvent::operator==(const CNewSmartTokenEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CNewSmartTokenEvent& v1, const CNewSmartTokenEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CNewSmartTokenEvent> CNewSmartTokenEventArray;
extern CArchive& operator>>(CArchive& archive, CNewSmartTokenEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CNewSmartTokenEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

