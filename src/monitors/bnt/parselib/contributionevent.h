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
class CContributionEvent : public CLogEntry {
public:
    address_t _contributor;
    biguint_t _amount;
    biguint_t _return;

public:
    CContributionEvent(void);
    CContributionEvent(const CContributionEvent& co);
    virtual ~CContributionEvent(void);
    CContributionEvent& operator=(const CContributionEvent& co);

    DECLARE_NODE(CContributionEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CContributionEvent& item) const;
    bool operator!=(const CContributionEvent& item) const { return !operator==(item); }
    friend bool operator<(const CContributionEvent& v1, const CContributionEvent& v2);
    friend ostream& operator<<(ostream& os, const CContributionEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CContributionEvent& co);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CContributionEvent::CContributionEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CContributionEvent::CContributionEvent(const CContributionEvent& co) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(co);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CContributionEvent::~CContributionEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CContributionEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CContributionEvent::initialize(void) {
    CLogEntry::initialize();

    _contributor = "";
    _amount = 0;
    _return = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CContributionEvent::duplicate(const CContributionEvent& co) {
    clear();
    CLogEntry::duplicate(co);

    _contributor = co._contributor;
    _amount = co._amount;
    _return = co._return;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CContributionEvent& CContributionEvent::operator=(const CContributionEvent& co) {
    duplicate(co);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CContributionEvent::operator==(const CContributionEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CContributionEvent& v1, const CContributionEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CContributionEvent> CContributionEventArray;
extern CArchive& operator>>(CArchive& archive, CContributionEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CContributionEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

