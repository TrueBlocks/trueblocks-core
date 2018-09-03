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
class CAuctionStartedEvent : public CLogEntry {
public:
    biguint_t _auction_start_time;
    biguint_t _block_number;

public:
    CAuctionStartedEvent(void);
    CAuctionStartedEvent(const CAuctionStartedEvent& au);
    virtual ~CAuctionStartedEvent(void);
    CAuctionStartedEvent& operator=(const CAuctionStartedEvent& au);

    DECLARE_NODE(CAuctionStartedEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CAuctionStartedEvent& item) const;
    bool operator!=(const CAuctionStartedEvent& item) const { return !operator==(item); }
    friend bool operator<(const CAuctionStartedEvent& v1, const CAuctionStartedEvent& v2);
    friend ostream& operator<<(ostream& os, const CAuctionStartedEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CAuctionStartedEvent& au);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAuctionStartedEvent::CAuctionStartedEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAuctionStartedEvent::CAuctionStartedEvent(const CAuctionStartedEvent& au) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(au);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAuctionStartedEvent::~CAuctionStartedEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAuctionStartedEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAuctionStartedEvent::initialize(void) {
    CLogEntry::initialize();

    _auction_start_time = 0;
    _block_number = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAuctionStartedEvent::duplicate(const CAuctionStartedEvent& au) {
    clear();
    CLogEntry::duplicate(au);

    _auction_start_time = au._auction_start_time;
    _block_number = au._block_number;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CAuctionStartedEvent& CAuctionStartedEvent::operator=(const CAuctionStartedEvent& au) {
    duplicate(au);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CAuctionStartedEvent::operator==(const CAuctionStartedEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CAuctionStartedEvent& v1, const CAuctionStartedEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CAuctionStartedEvent> CAuctionStartedEventArray;
extern CArchive& operator>>(CArchive& archive, CAuctionStartedEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CAuctionStartedEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

