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
class CConfirmationEvent : public CLogEntry {
public:
    address_t sender;
    biguint_t transactionId;

public:
    CConfirmationEvent(void);
    CConfirmationEvent(const CConfirmationEvent& co);
    virtual ~CConfirmationEvent(void);
    CConfirmationEvent& operator=(const CConfirmationEvent& co);

    DECLARE_NODE(CConfirmationEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CConfirmationEvent& item) const;
    bool operator!=(const CConfirmationEvent& item) const { return !operator==(item); }
    friend bool operator<(const CConfirmationEvent& v1, const CConfirmationEvent& v2);
    friend ostream& operator<<(ostream& os, const CConfirmationEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CConfirmationEvent& co);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CConfirmationEvent::CConfirmationEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CConfirmationEvent::CConfirmationEvent(const CConfirmationEvent& co) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(co);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CConfirmationEvent::~CConfirmationEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CConfirmationEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CConfirmationEvent::initialize(void) {
    CLogEntry::initialize();

    sender = "";
    transactionId = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CConfirmationEvent::duplicate(const CConfirmationEvent& co) {
    clear();
    CLogEntry::duplicate(co);

    sender = co.sender;
    transactionId = co.transactionId;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CConfirmationEvent& CConfirmationEvent::operator=(const CConfirmationEvent& co) {
    duplicate(co);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CConfirmationEvent::operator==(const CConfirmationEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CConfirmationEvent& v1, const CConfirmationEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CConfirmationEvent> CConfirmationEventArray;
extern CArchive& operator>>(CArchive& archive, CConfirmationEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CConfirmationEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

