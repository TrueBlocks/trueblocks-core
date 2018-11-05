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
class CBurnEvent : public CLogEntry {
public:
    address_t burner;
    biguint_t value;

public:
    CBurnEvent(void);
    CBurnEvent(const CBurnEvent& bu);
    virtual ~CBurnEvent(void);
    CBurnEvent& operator=(const CBurnEvent& bu);

    DECLARE_NODE(CBurnEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CBurnEvent& item) const;
    bool operator!=(const CBurnEvent& item) const { return !operator==(item); }
    friend bool operator<(const CBurnEvent& v1, const CBurnEvent& v2);
    friend ostream& operator<<(ostream& os, const CBurnEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CBurnEvent& bu);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CBurnEvent::CBurnEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CBurnEvent::CBurnEvent(const CBurnEvent& bu) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(bu);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CBurnEvent::~CBurnEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBurnEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBurnEvent::initialize(void) {
    CLogEntry::initialize();

    burner = "";
    value = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBurnEvent::duplicate(const CBurnEvent& bu) {
    clear();
    CLogEntry::duplicate(bu);

    burner = bu.burner;
    value = bu.value;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CBurnEvent& CBurnEvent::operator=(const CBurnEvent& bu) {
    duplicate(bu);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CBurnEvent::operator==(const CBurnEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CBurnEvent& v1, const CBurnEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CBurnEvent> CBurnEventArray;
extern CArchive& operator>>(CArchive& archive, CBurnEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CBurnEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

