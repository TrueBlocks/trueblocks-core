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
class CFreezeEvent : public CLogEntry {
public:
    address_t from;
    biguint_t value;

public:
    CFreezeEvent(void);
    CFreezeEvent(const CFreezeEvent& fr);
    virtual ~CFreezeEvent(void);
    CFreezeEvent& operator=(const CFreezeEvent& fr);

    DECLARE_NODE(CFreezeEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CFreezeEvent& item) const;
    bool operator!=(const CFreezeEvent& item) const { return !operator==(item); }
    friend bool operator<(const CFreezeEvent& v1, const CFreezeEvent& v2);
    friend ostream& operator<<(ostream& os, const CFreezeEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CFreezeEvent& fr);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CFreezeEvent::CFreezeEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CFreezeEvent::CFreezeEvent(const CFreezeEvent& fr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(fr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CFreezeEvent::~CFreezeEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFreezeEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFreezeEvent::initialize(void) {
    CLogEntry::initialize();

    from = "";
    value = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFreezeEvent::duplicate(const CFreezeEvent& fr) {
    clear();
    CLogEntry::duplicate(fr);

    from = fr.from;
    value = fr.value;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CFreezeEvent& CFreezeEvent::operator=(const CFreezeEvent& fr) {
    duplicate(fr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CFreezeEvent::operator==(const CFreezeEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CFreezeEvent& v1, const CFreezeEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CFreezeEvent> CFreezeEventArray;
extern CArchive& operator>>(CArchive& archive, CFreezeEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CFreezeEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

