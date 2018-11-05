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
class CCreatedTokenEvent : public CLogEntry {
public:
    address_t to;
    biguint_t amount;

public:
    CCreatedTokenEvent(void);
    CCreatedTokenEvent(const CCreatedTokenEvent& cr);
    virtual ~CCreatedTokenEvent(void);
    CCreatedTokenEvent& operator=(const CCreatedTokenEvent& cr);

    DECLARE_NODE(CCreatedTokenEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CCreatedTokenEvent& item) const;
    bool operator!=(const CCreatedTokenEvent& item) const { return !operator==(item); }
    friend bool operator<(const CCreatedTokenEvent& v1, const CCreatedTokenEvent& v2);
    friend ostream& operator<<(ostream& os, const CCreatedTokenEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CCreatedTokenEvent& cr);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CCreatedTokenEvent::CCreatedTokenEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CCreatedTokenEvent::CCreatedTokenEvent(const CCreatedTokenEvent& cr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(cr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CCreatedTokenEvent::~CCreatedTokenEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCreatedTokenEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCreatedTokenEvent::initialize(void) {
    CLogEntry::initialize();

    to = "";
    amount = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCreatedTokenEvent::duplicate(const CCreatedTokenEvent& cr) {
    clear();
    CLogEntry::duplicate(cr);

    to = cr.to;
    amount = cr.amount;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CCreatedTokenEvent& CCreatedTokenEvent::operator=(const CCreatedTokenEvent& cr) {
    duplicate(cr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CCreatedTokenEvent::operator==(const CCreatedTokenEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CCreatedTokenEvent& v1, const CCreatedTokenEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CCreatedTokenEvent> CCreatedTokenEventArray;
extern CArchive& operator>>(CArchive& archive, CCreatedTokenEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CCreatedTokenEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

