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
class CRefundReceivedEvent : public CLogEntry {
public:
    biguint_t amount;

public:
    CRefundReceivedEvent(void);
    CRefundReceivedEvent(const CRefundReceivedEvent& re);
    virtual ~CRefundReceivedEvent(void);
    CRefundReceivedEvent& operator=(const CRefundReceivedEvent& re);

    DECLARE_NODE(CRefundReceivedEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CRefundReceivedEvent& item) const;
    bool operator!=(const CRefundReceivedEvent& item) const { return !operator==(item); }
    friend bool operator<(const CRefundReceivedEvent& v1, const CRefundReceivedEvent& v2);
    friend ostream& operator<<(ostream& os, const CRefundReceivedEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CRefundReceivedEvent& re);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CRefundReceivedEvent::CRefundReceivedEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CRefundReceivedEvent::CRefundReceivedEvent(const CRefundReceivedEvent& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CRefundReceivedEvent::~CRefundReceivedEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRefundReceivedEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRefundReceivedEvent::initialize(void) {
    CLogEntry::initialize();

    amount = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRefundReceivedEvent::duplicate(const CRefundReceivedEvent& re) {
    clear();
    CLogEntry::duplicate(re);

    amount = re.amount;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CRefundReceivedEvent& CRefundReceivedEvent::operator=(const CRefundReceivedEvent& re) {
    duplicate(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CRefundReceivedEvent::operator==(const CRefundReceivedEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CRefundReceivedEvent& v1, const CRefundReceivedEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CRefundReceivedEvent> CRefundReceivedEventArray;
extern CArchive& operator>>(CArchive& archive, CRefundReceivedEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CRefundReceivedEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

