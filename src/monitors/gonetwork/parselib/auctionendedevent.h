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
class CAuctionEndedEvent : public CLogEntry {
public:
    biguint_t _final_price;

public:
    CAuctionEndedEvent(void);
    CAuctionEndedEvent(const CAuctionEndedEvent& au);
    virtual ~CAuctionEndedEvent(void);
    CAuctionEndedEvent& operator=(const CAuctionEndedEvent& au);

    DECLARE_NODE(CAuctionEndedEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CAuctionEndedEvent& item) const;
    bool operator!=(const CAuctionEndedEvent& item) const { return !operator==(item); }
    friend bool operator<(const CAuctionEndedEvent& v1, const CAuctionEndedEvent& v2);
    friend ostream& operator<<(ostream& os, const CAuctionEndedEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CAuctionEndedEvent& au);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAuctionEndedEvent::CAuctionEndedEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAuctionEndedEvent::CAuctionEndedEvent(const CAuctionEndedEvent& au) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(au);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAuctionEndedEvent::~CAuctionEndedEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAuctionEndedEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAuctionEndedEvent::initialize(void) {
    CLogEntry::initialize();

    _final_price = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAuctionEndedEvent::duplicate(const CAuctionEndedEvent& au) {
    clear();
    CLogEntry::duplicate(au);

    _final_price = au._final_price;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CAuctionEndedEvent& CAuctionEndedEvent::operator=(const CAuctionEndedEvent& au) {
    duplicate(au);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CAuctionEndedEvent::operator==(const CAuctionEndedEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CAuctionEndedEvent& v1, const CAuctionEndedEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CAuctionEndedEvent> CAuctionEndedEventArray;
extern CArchive& operator>>(CArchive& archive, CAuctionEndedEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CAuctionEndedEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

