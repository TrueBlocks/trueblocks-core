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
class CFuelingToDateEvent : public CLogEntry {
public:
    biguint_t value;

public:
    CFuelingToDateEvent(void);
    CFuelingToDateEvent(const CFuelingToDateEvent& fu);
    virtual ~CFuelingToDateEvent(void);
    CFuelingToDateEvent& operator=(const CFuelingToDateEvent& fu);

    DECLARE_NODE(CFuelingToDateEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CFuelingToDateEvent& item) const;
    bool operator!=(const CFuelingToDateEvent& item) const { return !operator==(item); }
    friend bool operator<(const CFuelingToDateEvent& v1, const CFuelingToDateEvent& v2);
    friend ostream& operator<<(ostream& os, const CFuelingToDateEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CFuelingToDateEvent& fu);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CFuelingToDateEvent::CFuelingToDateEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CFuelingToDateEvent::CFuelingToDateEvent(const CFuelingToDateEvent& fu) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(fu);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CFuelingToDateEvent::~CFuelingToDateEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFuelingToDateEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFuelingToDateEvent::initialize(void) {
    CLogEntry::initialize();

    value = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFuelingToDateEvent::duplicate(const CFuelingToDateEvent& fu) {
    clear();
    CLogEntry::duplicate(fu);

    value = fu.value;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CFuelingToDateEvent& CFuelingToDateEvent::operator=(const CFuelingToDateEvent& fu) {
    duplicate(fu);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CFuelingToDateEvent::operator==(const CFuelingToDateEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CFuelingToDateEvent& v1, const CFuelingToDateEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CFuelingToDateEvent> CFuelingToDateEventArray;
extern CArchive& operator>>(CArchive& archive, CFuelingToDateEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CFuelingToDateEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

