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
class CRevokedEvent : public CLogEntry {
public:
    address_t who;

public:
    CRevokedEvent(void);
    CRevokedEvent(const CRevokedEvent& re);
    virtual ~CRevokedEvent(void);
    CRevokedEvent& operator=(const CRevokedEvent& re);

    DECLARE_NODE(CRevokedEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CRevokedEvent& item) const;
    bool operator!=(const CRevokedEvent& item) const { return !operator==(item); }
    friend bool operator<(const CRevokedEvent& v1, const CRevokedEvent& v2);
    friend ostream& operator<<(ostream& os, const CRevokedEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CRevokedEvent& re);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CRevokedEvent::CRevokedEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CRevokedEvent::CRevokedEvent(const CRevokedEvent& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CRevokedEvent::~CRevokedEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRevokedEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRevokedEvent::initialize(void) {
    CLogEntry::initialize();

    who = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRevokedEvent::duplicate(const CRevokedEvent& re) {
    clear();
    CLogEntry::duplicate(re);

    who = re.who;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CRevokedEvent& CRevokedEvent::operator=(const CRevokedEvent& re) {
    duplicate(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CRevokedEvent::operator==(const CRevokedEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CRevokedEvent& v1, const CRevokedEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CRevokedEvent> CRevokedEventArray;
extern CArchive& operator>>(CArchive& archive, CRevokedEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CRevokedEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

