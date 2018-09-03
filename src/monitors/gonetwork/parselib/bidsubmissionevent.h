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
class CBidSubmissionEvent : public CLogEntry {
public:
    address_t _sender;
    biguint_t _amount;
    biguint_t _amount_with_bonus;
    biguint_t _remaining_funds_to_end_auction;

public:
    CBidSubmissionEvent(void);
    CBidSubmissionEvent(const CBidSubmissionEvent& bi);
    virtual ~CBidSubmissionEvent(void);
    CBidSubmissionEvent& operator=(const CBidSubmissionEvent& bi);

    DECLARE_NODE(CBidSubmissionEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CBidSubmissionEvent& item) const;
    bool operator!=(const CBidSubmissionEvent& item) const { return !operator==(item); }
    friend bool operator<(const CBidSubmissionEvent& v1, const CBidSubmissionEvent& v2);
    friend ostream& operator<<(ostream& os, const CBidSubmissionEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CBidSubmissionEvent& bi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CBidSubmissionEvent::CBidSubmissionEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CBidSubmissionEvent::CBidSubmissionEvent(const CBidSubmissionEvent& bi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(bi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CBidSubmissionEvent::~CBidSubmissionEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBidSubmissionEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBidSubmissionEvent::initialize(void) {
    CLogEntry::initialize();

    _sender = "";
    _amount = 0;
    _amount_with_bonus = 0;
    _remaining_funds_to_end_auction = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBidSubmissionEvent::duplicate(const CBidSubmissionEvent& bi) {
    clear();
    CLogEntry::duplicate(bi);

    _sender = bi._sender;
    _amount = bi._amount;
    _amount_with_bonus = bi._amount_with_bonus;
    _remaining_funds_to_end_auction = bi._remaining_funds_to_end_auction;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CBidSubmissionEvent& CBidSubmissionEvent::operator=(const CBidSubmissionEvent& bi) {
    duplicate(bi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CBidSubmissionEvent::operator==(const CBidSubmissionEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CBidSubmissionEvent& v1, const CBidSubmissionEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CBidSubmissionEvent> CBidSubmissionEventArray;
extern CArchive& operator>>(CArchive& archive, CBidSubmissionEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CBidSubmissionEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

