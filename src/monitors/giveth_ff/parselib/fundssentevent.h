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
class CFundsSentEvent : public CLogEntry {
public:
    address_t sender;
    biguint_t amount;

public:
    CFundsSentEvent(void);
    CFundsSentEvent(const CFundsSentEvent& fu);
    virtual ~CFundsSentEvent(void);
    CFundsSentEvent& operator=(const CFundsSentEvent& fu);

    DECLARE_NODE(CFundsSentEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CFundsSentEvent& item) const;
    bool operator!=(const CFundsSentEvent& item) const { return !operator==(item); }
    friend bool operator<(const CFundsSentEvent& v1, const CFundsSentEvent& v2);
    friend ostream& operator<<(ostream& os, const CFundsSentEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CFundsSentEvent& fu);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CFundsSentEvent::CFundsSentEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CFundsSentEvent::CFundsSentEvent(const CFundsSentEvent& fu) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(fu);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CFundsSentEvent::~CFundsSentEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFundsSentEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFundsSentEvent::initialize(void) {
    CLogEntry::initialize();

    sender = "";
    amount = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFundsSentEvent::duplicate(const CFundsSentEvent& fu) {
    clear();
    CLogEntry::duplicate(fu);

    sender = fu.sender;
    amount = fu.amount;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CFundsSentEvent& CFundsSentEvent::operator=(const CFundsSentEvent& fu) {
    duplicate(fu);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CFundsSentEvent::operator==(const CFundsSentEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CFundsSentEvent& v1, const CFundsSentEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CFundsSentEvent> CFundsSentEventArray;
extern CArchive& operator>>(CArchive& archive, CFundsSentEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CFundsSentEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

