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
class CBaseTransferedEvent : public CLogEntry {
public:
    string_q _base;
    address_t _to;

public:
    CBaseTransferedEvent(void);
    CBaseTransferedEvent(const CBaseTransferedEvent& ba);
    virtual ~CBaseTransferedEvent(void);
    CBaseTransferedEvent& operator=(const CBaseTransferedEvent& ba);

    DECLARE_NODE(CBaseTransferedEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CBaseTransferedEvent& item) const;
    bool operator!=(const CBaseTransferedEvent& item) const { return !operator==(item); }
    friend bool operator<(const CBaseTransferedEvent& v1, const CBaseTransferedEvent& v2);
    friend ostream& operator<<(ostream& os, const CBaseTransferedEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CBaseTransferedEvent& ba);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CBaseTransferedEvent::CBaseTransferedEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CBaseTransferedEvent::CBaseTransferedEvent(const CBaseTransferedEvent& ba) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ba);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CBaseTransferedEvent::~CBaseTransferedEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBaseTransferedEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBaseTransferedEvent::initialize(void) {
    CLogEntry::initialize();

    _base = "";
    _to = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBaseTransferedEvent::duplicate(const CBaseTransferedEvent& ba) {
    clear();
    CLogEntry::duplicate(ba);

    _base = ba._base;
    _to = ba._to;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CBaseTransferedEvent& CBaseTransferedEvent::operator=(const CBaseTransferedEvent& ba) {
    duplicate(ba);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CBaseTransferedEvent::operator==(const CBaseTransferedEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CBaseTransferedEvent& v1, const CBaseTransferedEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CBaseTransferedEvent> CBaseTransferedEventArray;
extern CArchive& operator>>(CArchive& archive, CBaseTransferedEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CBaseTransferedEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

