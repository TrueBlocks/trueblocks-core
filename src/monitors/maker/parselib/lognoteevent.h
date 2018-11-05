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
class CLogNoteEvent : public CLogEntry {
public:
    string_q sig;
    address_t guy;
    string_q foo;
    string_q bar;
    biguint_t wad;
    string_q fax;

public:
    CLogNoteEvent(void);
    CLogNoteEvent(const CLogNoteEvent& lo);
    virtual ~CLogNoteEvent(void);
    CLogNoteEvent& operator=(const CLogNoteEvent& lo);

    DECLARE_NODE(CLogNoteEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CLogNoteEvent& item) const;
    bool operator!=(const CLogNoteEvent& item) const { return !operator==(item); }
    friend bool operator<(const CLogNoteEvent& v1, const CLogNoteEvent& v2);
    friend ostream& operator<<(ostream& os, const CLogNoteEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CLogNoteEvent& lo);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CLogNoteEvent::CLogNoteEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CLogNoteEvent::CLogNoteEvent(const CLogNoteEvent& lo) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(lo);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CLogNoteEvent::~CLogNoteEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CLogNoteEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CLogNoteEvent::initialize(void) {
    CLogEntry::initialize();

    sig = "";
    guy = "";
    foo = "";
    bar = "";
    wad = 0;
    fax = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CLogNoteEvent::duplicate(const CLogNoteEvent& lo) {
    clear();
    CLogEntry::duplicate(lo);

    sig = lo.sig;
    guy = lo.guy;
    foo = lo.foo;
    bar = lo.bar;
    wad = lo.wad;
    fax = lo.fax;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CLogNoteEvent& CLogNoteEvent::operator=(const CLogNoteEvent& lo) {
    duplicate(lo);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CLogNoteEvent::operator==(const CLogNoteEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CLogNoteEvent& v1, const CLogNoteEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CLogNoteEvent> CLogNoteEventArray;
extern CArchive& operator>>(CArchive& archive, CLogNoteEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CLogNoteEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

