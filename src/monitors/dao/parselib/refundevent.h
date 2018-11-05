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
class CRefundEvent : public CLogEntry {
public:
    address_t to;
    biguint_t value;

public:
    CRefundEvent(void);
    CRefundEvent(const CRefundEvent& re);
    virtual ~CRefundEvent(void);
    CRefundEvent& operator=(const CRefundEvent& re);

    DECLARE_NODE(CRefundEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CRefundEvent& item) const;
    bool operator!=(const CRefundEvent& item) const { return !operator==(item); }
    friend bool operator<(const CRefundEvent& v1, const CRefundEvent& v2);
    friend ostream& operator<<(ostream& os, const CRefundEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CRefundEvent& re);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CRefundEvent::CRefundEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CRefundEvent::CRefundEvent(const CRefundEvent& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CRefundEvent::~CRefundEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRefundEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRefundEvent::initialize(void) {
    CLogEntry::initialize();

    to = "";
    value = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRefundEvent::duplicate(const CRefundEvent& re) {
    clear();
    CLogEntry::duplicate(re);

    to = re.to;
    value = re.value;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CRefundEvent& CRefundEvent::operator=(const CRefundEvent& re) {
    duplicate(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CRefundEvent::operator==(const CRefundEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CRefundEvent& v1, const CRefundEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CRefundEvent> CRefundEventArray;
extern CArchive& operator>>(CArchive& archive, CRefundEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CRefundEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

