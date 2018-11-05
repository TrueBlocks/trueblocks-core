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
class CNewTokenGrantEvent : public CLogEntry {
public:
    address_t from;
    address_t to;
    biguint_t value;
    uint64_t start;
    uint64_t cliff;
    uint64_t vesting;

public:
    CNewTokenGrantEvent(void);
    CNewTokenGrantEvent(const CNewTokenGrantEvent& ne);
    virtual ~CNewTokenGrantEvent(void);
    CNewTokenGrantEvent& operator=(const CNewTokenGrantEvent& ne);

    DECLARE_NODE(CNewTokenGrantEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CNewTokenGrantEvent& item) const;
    bool operator!=(const CNewTokenGrantEvent& item) const { return !operator==(item); }
    friend bool operator<(const CNewTokenGrantEvent& v1, const CNewTokenGrantEvent& v2);
    friend ostream& operator<<(ostream& os, const CNewTokenGrantEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CNewTokenGrantEvent& ne);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CNewTokenGrantEvent::CNewTokenGrantEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CNewTokenGrantEvent::CNewTokenGrantEvent(const CNewTokenGrantEvent& ne) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ne);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CNewTokenGrantEvent::~CNewTokenGrantEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewTokenGrantEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewTokenGrantEvent::initialize(void) {
    CLogEntry::initialize();

    from = "";
    to = "";
    value = 0;
    start = 0;
    cliff = 0;
    vesting = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewTokenGrantEvent::duplicate(const CNewTokenGrantEvent& ne) {
    clear();
    CLogEntry::duplicate(ne);

    from = ne.from;
    to = ne.to;
    value = ne.value;
    start = ne.start;
    cliff = ne.cliff;
    vesting = ne.vesting;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CNewTokenGrantEvent& CNewTokenGrantEvent::operator=(const CNewTokenGrantEvent& ne) {
    duplicate(ne);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CNewTokenGrantEvent::operator==(const CNewTokenGrantEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CNewTokenGrantEvent& v1, const CNewTokenGrantEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CNewTokenGrantEvent> CNewTokenGrantEventArray;
extern CArchive& operator>>(CArchive& archive, CNewTokenGrantEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CNewTokenGrantEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

