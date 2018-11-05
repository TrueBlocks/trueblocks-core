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
class CIssuanceEvent : public CLogEntry {
public:
    biguint_t _amount;

public:
    CIssuanceEvent(void);
    CIssuanceEvent(const CIssuanceEvent& is);
    virtual ~CIssuanceEvent(void);
    CIssuanceEvent& operator=(const CIssuanceEvent& is);

    DECLARE_NODE(CIssuanceEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CIssuanceEvent& item) const;
    bool operator!=(const CIssuanceEvent& item) const { return !operator==(item); }
    friend bool operator<(const CIssuanceEvent& v1, const CIssuanceEvent& v2);
    friend ostream& operator<<(ostream& os, const CIssuanceEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CIssuanceEvent& is);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CIssuanceEvent::CIssuanceEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CIssuanceEvent::CIssuanceEvent(const CIssuanceEvent& is) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(is);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CIssuanceEvent::~CIssuanceEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CIssuanceEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CIssuanceEvent::initialize(void) {
    CLogEntry::initialize();

    _amount = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CIssuanceEvent::duplicate(const CIssuanceEvent& is) {
    clear();
    CLogEntry::duplicate(is);

    _amount = is._amount;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CIssuanceEvent& CIssuanceEvent::operator=(const CIssuanceEvent& is) {
    duplicate(is);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CIssuanceEvent::operator==(const CIssuanceEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CIssuanceEvent& v1, const CIssuanceEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CIssuanceEvent> CIssuanceEventArray;
extern CArchive& operator>>(CArchive& archive, CIssuanceEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CIssuanceEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

