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
class CAffirmStatusEvent : public CLogEntry {
public:
    address_t _sender;
    address_t _account;
    biguint_t _affirmDate;
    biguint_t _value;

public:
    CAffirmStatusEvent(void);
    CAffirmStatusEvent(const CAffirmStatusEvent& af);
    virtual ~CAffirmStatusEvent(void);
    CAffirmStatusEvent& operator=(const CAffirmStatusEvent& af);

    DECLARE_NODE(CAffirmStatusEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CAffirmStatusEvent& item) const;
    bool operator!=(const CAffirmStatusEvent& item) const { return !operator==(item); }
    friend bool operator<(const CAffirmStatusEvent& v1, const CAffirmStatusEvent& v2);
    friend ostream& operator<<(ostream& os, const CAffirmStatusEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CAffirmStatusEvent& af);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAffirmStatusEvent::CAffirmStatusEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAffirmStatusEvent::CAffirmStatusEvent(const CAffirmStatusEvent& af) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(af);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAffirmStatusEvent::~CAffirmStatusEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAffirmStatusEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAffirmStatusEvent::initialize(void) {
    CLogEntry::initialize();

    _sender = "";
    _account = "";
    _affirmDate = 0;
    _value = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAffirmStatusEvent::duplicate(const CAffirmStatusEvent& af) {
    clear();
    CLogEntry::duplicate(af);

    _sender = af._sender;
    _account = af._account;
    _affirmDate = af._affirmDate;
    _value = af._value;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CAffirmStatusEvent& CAffirmStatusEvent::operator=(const CAffirmStatusEvent& af) {
    duplicate(af);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CAffirmStatusEvent::operator==(const CAffirmStatusEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CAffirmStatusEvent& v1, const CAffirmStatusEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CAffirmStatusEvent> CAffirmStatusEventArray;
extern CArchive& operator>>(CArchive& archive, CAffirmStatusEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CAffirmStatusEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

