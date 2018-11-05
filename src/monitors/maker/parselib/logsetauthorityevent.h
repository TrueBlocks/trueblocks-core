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
class CLogSetAuthorityEvent : public CLogEntry {
public:
    address_t authority;

public:
    CLogSetAuthorityEvent(void);
    CLogSetAuthorityEvent(const CLogSetAuthorityEvent& lo);
    virtual ~CLogSetAuthorityEvent(void);
    CLogSetAuthorityEvent& operator=(const CLogSetAuthorityEvent& lo);

    DECLARE_NODE(CLogSetAuthorityEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CLogSetAuthorityEvent& item) const;
    bool operator!=(const CLogSetAuthorityEvent& item) const { return !operator==(item); }
    friend bool operator<(const CLogSetAuthorityEvent& v1, const CLogSetAuthorityEvent& v2);
    friend ostream& operator<<(ostream& os, const CLogSetAuthorityEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CLogSetAuthorityEvent& lo);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CLogSetAuthorityEvent::CLogSetAuthorityEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CLogSetAuthorityEvent::CLogSetAuthorityEvent(const CLogSetAuthorityEvent& lo) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(lo);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CLogSetAuthorityEvent::~CLogSetAuthorityEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CLogSetAuthorityEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CLogSetAuthorityEvent::initialize(void) {
    CLogEntry::initialize();

    authority = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CLogSetAuthorityEvent::duplicate(const CLogSetAuthorityEvent& lo) {
    clear();
    CLogEntry::duplicate(lo);

    authority = lo.authority;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CLogSetAuthorityEvent& CLogSetAuthorityEvent::operator=(const CLogSetAuthorityEvent& lo) {
    duplicate(lo);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CLogSetAuthorityEvent::operator==(const CLogSetAuthorityEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CLogSetAuthorityEvent& v1, const CLogSetAuthorityEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CLogSetAuthorityEvent> CLogSetAuthorityEventArray;
extern CArchive& operator>>(CArchive& archive, CLogSetAuthorityEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CLogSetAuthorityEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

