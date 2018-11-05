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
class CRefundSubmissionEvent : public CLogEntry {
public:
    address_t sender;
    biguint_t amount;

public:
    CRefundSubmissionEvent(void);
    CRefundSubmissionEvent(const CRefundSubmissionEvent& re);
    virtual ~CRefundSubmissionEvent(void);
    CRefundSubmissionEvent& operator=(const CRefundSubmissionEvent& re);

    DECLARE_NODE(CRefundSubmissionEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CRefundSubmissionEvent& item) const;
    bool operator!=(const CRefundSubmissionEvent& item) const { return !operator==(item); }
    friend bool operator<(const CRefundSubmissionEvent& v1, const CRefundSubmissionEvent& v2);
    friend ostream& operator<<(ostream& os, const CRefundSubmissionEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CRefundSubmissionEvent& re);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CRefundSubmissionEvent::CRefundSubmissionEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CRefundSubmissionEvent::CRefundSubmissionEvent(const CRefundSubmissionEvent& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CRefundSubmissionEvent::~CRefundSubmissionEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRefundSubmissionEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRefundSubmissionEvent::initialize(void) {
    CLogEntry::initialize();

    sender = "";
    amount = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRefundSubmissionEvent::duplicate(const CRefundSubmissionEvent& re) {
    clear();
    CLogEntry::duplicate(re);

    sender = re.sender;
    amount = re.amount;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CRefundSubmissionEvent& CRefundSubmissionEvent::operator=(const CRefundSubmissionEvent& re) {
    duplicate(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CRefundSubmissionEvent::operator==(const CRefundSubmissionEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CRefundSubmissionEvent& v1, const CRefundSubmissionEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CRefundSubmissionEvent> CRefundSubmissionEventArray;
extern CArchive& operator>>(CArchive& archive, CRefundSubmissionEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CRefundSubmissionEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

