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
class CMintEvent : public CLogEntry {
public:
    address_t guy;
    biguint_t wad;

public:
    CMintEvent(void);
    CMintEvent(const CMintEvent& mi);
    virtual ~CMintEvent(void);
    CMintEvent& operator=(const CMintEvent& mi);

    DECLARE_NODE(CMintEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CMintEvent& item) const;
    bool operator!=(const CMintEvent& item) const { return !operator==(item); }
    friend bool operator<(const CMintEvent& v1, const CMintEvent& v2);
    friend ostream& operator<<(ostream& os, const CMintEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CMintEvent& mi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CMintEvent::CMintEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CMintEvent::CMintEvent(const CMintEvent& mi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(mi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CMintEvent::~CMintEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMintEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMintEvent::initialize(void) {
    CLogEntry::initialize();

    guy = "";
    wad = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMintEvent::duplicate(const CMintEvent& mi) {
    clear();
    CLogEntry::duplicate(mi);

    guy = mi.guy;
    wad = mi.wad;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CMintEvent& CMintEvent::operator=(const CMintEvent& mi) {
    duplicate(mi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CMintEvent::operator==(const CMintEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CMintEvent& v1, const CMintEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CMintEvent> CMintEventArray;
extern CArchive& operator>>(CArchive& archive, CMintEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CMintEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

