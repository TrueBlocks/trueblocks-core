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
class CCapReachedEvent : public CLogEntry {
public:
    address_t _beneficiary;
    biguint_t _amountRaised;

public:
    CCapReachedEvent(void);
    CCapReachedEvent(const CCapReachedEvent& ca);
    virtual ~CCapReachedEvent(void);
    CCapReachedEvent& operator=(const CCapReachedEvent& ca);

    DECLARE_NODE(CCapReachedEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CCapReachedEvent& item) const;
    bool operator!=(const CCapReachedEvent& item) const { return !operator==(item); }
    friend bool operator<(const CCapReachedEvent& v1, const CCapReachedEvent& v2);
    friend ostream& operator<<(ostream& os, const CCapReachedEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CCapReachedEvent& ca);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CCapReachedEvent::CCapReachedEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CCapReachedEvent::CCapReachedEvent(const CCapReachedEvent& ca) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ca);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CCapReachedEvent::~CCapReachedEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCapReachedEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCapReachedEvent::initialize(void) {
    CLogEntry::initialize();

    _beneficiary = "";
    _amountRaised = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCapReachedEvent::duplicate(const CCapReachedEvent& ca) {
    clear();
    CLogEntry::duplicate(ca);

    _beneficiary = ca._beneficiary;
    _amountRaised = ca._amountRaised;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CCapReachedEvent& CCapReachedEvent::operator=(const CCapReachedEvent& ca) {
    duplicate(ca);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CCapReachedEvent::operator==(const CCapReachedEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CCapReachedEvent& v1, const CCapReachedEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CCapReachedEvent> CCapReachedEventArray;
extern CArchive& operator>>(CArchive& archive, CCapReachedEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CCapReachedEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

