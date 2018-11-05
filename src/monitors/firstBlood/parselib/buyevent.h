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
class CBuyEvent : public CLogEntry {
public:
    address_t sender;
    biguint_t eth;
    biguint_t fbt;

public:
    CBuyEvent(void);
    CBuyEvent(const CBuyEvent& bu);
    virtual ~CBuyEvent(void);
    CBuyEvent& operator=(const CBuyEvent& bu);

    DECLARE_NODE(CBuyEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CBuyEvent& item) const;
    bool operator!=(const CBuyEvent& item) const { return !operator==(item); }
    friend bool operator<(const CBuyEvent& v1, const CBuyEvent& v2);
    friend ostream& operator<<(ostream& os, const CBuyEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CBuyEvent& bu);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CBuyEvent::CBuyEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CBuyEvent::CBuyEvent(const CBuyEvent& bu) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(bu);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CBuyEvent::~CBuyEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBuyEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBuyEvent::initialize(void) {
    CLogEntry::initialize();

    sender = "";
    eth = 0;
    fbt = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBuyEvent::duplicate(const CBuyEvent& bu) {
    clear();
    CLogEntry::duplicate(bu);

    sender = bu.sender;
    eth = bu.eth;
    fbt = bu.fbt;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CBuyEvent& CBuyEvent::operator=(const CBuyEvent& bu) {
    duplicate(bu);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CBuyEvent::operator==(const CBuyEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CBuyEvent& v1, const CBuyEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CBuyEvent> CBuyEventArray;
extern CArchive& operator>>(CArchive& archive, CBuyEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CBuyEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

