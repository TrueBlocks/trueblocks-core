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
class CUnpauseEvent : public CLogEntry {
public:
    CUnpauseEvent(void);
    CUnpauseEvent(const CUnpauseEvent& un);
    virtual ~CUnpauseEvent(void);
    CUnpauseEvent& operator=(const CUnpauseEvent& un);

    DECLARE_NODE(CUnpauseEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CUnpauseEvent& item) const;
    bool operator!=(const CUnpauseEvent& item) const { return !operator==(item); }
    friend bool operator<(const CUnpauseEvent& v1, const CUnpauseEvent& v2);
    friend ostream& operator<<(ostream& os, const CUnpauseEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CUnpauseEvent& un);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CUnpauseEvent::CUnpauseEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CUnpauseEvent::CUnpauseEvent(const CUnpauseEvent& un) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(un);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CUnpauseEvent::~CUnpauseEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CUnpauseEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CUnpauseEvent::initialize(void) {
    CLogEntry::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CUnpauseEvent::duplicate(const CUnpauseEvent& un) {
    clear();
    CLogEntry::duplicate(un);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CUnpauseEvent& CUnpauseEvent::operator=(const CUnpauseEvent& un) {
    duplicate(un);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CUnpauseEvent::operator==(const CUnpauseEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CUnpauseEvent& v1, const CUnpauseEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CUnpauseEvent> CUnpauseEventArray;
extern CArchive& operator>>(CArchive& archive, CUnpauseEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CUnpauseEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

