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
class CConfirmedEvent : public CLogEntry {
public:
    address_t who;

public:
    CConfirmedEvent(void);
    CConfirmedEvent(const CConfirmedEvent& co);
    virtual ~CConfirmedEvent(void);
    CConfirmedEvent& operator=(const CConfirmedEvent& co);

    DECLARE_NODE(CConfirmedEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CConfirmedEvent& item) const;
    bool operator!=(const CConfirmedEvent& item) const { return !operator==(item); }
    friend bool operator<(const CConfirmedEvent& v1, const CConfirmedEvent& v2);
    friend ostream& operator<<(ostream& os, const CConfirmedEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CConfirmedEvent& co);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CConfirmedEvent::CConfirmedEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CConfirmedEvent::CConfirmedEvent(const CConfirmedEvent& co) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(co);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CConfirmedEvent::~CConfirmedEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CConfirmedEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CConfirmedEvent::initialize(void) {
    CLogEntry::initialize();

    who = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CConfirmedEvent::duplicate(const CConfirmedEvent& co) {
    clear();
    CLogEntry::duplicate(co);

    who = co.who;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CConfirmedEvent& CConfirmedEvent::operator=(const CConfirmedEvent& co) {
    duplicate(co);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CConfirmedEvent::operator==(const CConfirmedEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CConfirmedEvent& v1, const CConfirmedEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CConfirmedEvent> CConfirmedEventArray;
extern CArchive& operator>>(CArchive& archive, CConfirmedEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CConfirmedEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

