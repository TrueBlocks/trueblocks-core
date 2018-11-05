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
class CRevocationEvent : public CLogEntry {
public:
    address_t sender;
    biguint_t transactionId;

public:
    CRevocationEvent(void);
    CRevocationEvent(const CRevocationEvent& re);
    virtual ~CRevocationEvent(void);
    CRevocationEvent& operator=(const CRevocationEvent& re);

    DECLARE_NODE(CRevocationEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CRevocationEvent& item) const;
    bool operator!=(const CRevocationEvent& item) const { return !operator==(item); }
    friend bool operator<(const CRevocationEvent& v1, const CRevocationEvent& v2);
    friend ostream& operator<<(ostream& os, const CRevocationEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CRevocationEvent& re);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CRevocationEvent::CRevocationEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CRevocationEvent::CRevocationEvent(const CRevocationEvent& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CRevocationEvent::~CRevocationEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRevocationEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRevocationEvent::initialize(void) {
    CLogEntry::initialize();

    sender = "";
    transactionId = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRevocationEvent::duplicate(const CRevocationEvent& re) {
    clear();
    CLogEntry::duplicate(re);

    sender = re.sender;
    transactionId = re.transactionId;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CRevocationEvent& CRevocationEvent::operator=(const CRevocationEvent& re) {
    duplicate(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CRevocationEvent::operator==(const CRevocationEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CRevocationEvent& v1, const CRevocationEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CRevocationEvent> CRevocationEventArray;
extern CArchive& operator>>(CArchive& archive, CRevocationEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CRevocationEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

