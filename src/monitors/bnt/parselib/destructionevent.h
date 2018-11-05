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
class CDestructionEvent : public CLogEntry {
public:
    biguint_t _amount;

public:
    CDestructionEvent(void);
    CDestructionEvent(const CDestructionEvent& de);
    virtual ~CDestructionEvent(void);
    CDestructionEvent& operator=(const CDestructionEvent& de);

    DECLARE_NODE(CDestructionEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CDestructionEvent& item) const;
    bool operator!=(const CDestructionEvent& item) const { return !operator==(item); }
    friend bool operator<(const CDestructionEvent& v1, const CDestructionEvent& v2);
    friend ostream& operator<<(ostream& os, const CDestructionEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CDestructionEvent& de);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CDestructionEvent::CDestructionEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CDestructionEvent::CDestructionEvent(const CDestructionEvent& de) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(de);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CDestructionEvent::~CDestructionEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDestructionEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDestructionEvent::initialize(void) {
    CLogEntry::initialize();

    _amount = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDestructionEvent::duplicate(const CDestructionEvent& de) {
    clear();
    CLogEntry::duplicate(de);

    _amount = de._amount;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CDestructionEvent& CDestructionEvent::operator=(const CDestructionEvent& de) {
    duplicate(de);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CDestructionEvent::operator==(const CDestructionEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CDestructionEvent& v1, const CDestructionEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CDestructionEvent> CDestructionEventArray;
extern CArchive& operator>>(CArchive& archive, CDestructionEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CDestructionEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

