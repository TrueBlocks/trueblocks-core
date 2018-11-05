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
class CPauseEvent : public CLogEntry {
public:
    CPauseEvent(void);
    CPauseEvent(const CPauseEvent& pa);
    virtual ~CPauseEvent(void);
    CPauseEvent& operator=(const CPauseEvent& pa);

    DECLARE_NODE(CPauseEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CPauseEvent& item) const;
    bool operator!=(const CPauseEvent& item) const { return !operator==(item); }
    friend bool operator<(const CPauseEvent& v1, const CPauseEvent& v2);
    friend ostream& operator<<(ostream& os, const CPauseEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CPauseEvent& pa);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CPauseEvent::CPauseEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPauseEvent::CPauseEvent(const CPauseEvent& pa) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(pa);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CPauseEvent::~CPauseEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPauseEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPauseEvent::initialize(void) {
    CLogEntry::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPauseEvent::duplicate(const CPauseEvent& pa) {
    clear();
    CLogEntry::duplicate(pa);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CPauseEvent& CPauseEvent::operator=(const CPauseEvent& pa) {
    duplicate(pa);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CPauseEvent::operator==(const CPauseEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CPauseEvent& v1, const CPauseEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CPauseEvent> CPauseEventArray;
extern CArchive& operator>>(CArchive& archive, CPauseEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CPauseEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

