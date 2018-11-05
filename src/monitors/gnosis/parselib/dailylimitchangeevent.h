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
class CDailyLimitChangeEvent : public CLogEntry {
public:
    biguint_t dailyLimit;

public:
    CDailyLimitChangeEvent(void);
    CDailyLimitChangeEvent(const CDailyLimitChangeEvent& da);
    virtual ~CDailyLimitChangeEvent(void);
    CDailyLimitChangeEvent& operator=(const CDailyLimitChangeEvent& da);

    DECLARE_NODE(CDailyLimitChangeEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CDailyLimitChangeEvent& item) const;
    bool operator!=(const CDailyLimitChangeEvent& item) const { return !operator==(item); }
    friend bool operator<(const CDailyLimitChangeEvent& v1, const CDailyLimitChangeEvent& v2);
    friend ostream& operator<<(ostream& os, const CDailyLimitChangeEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CDailyLimitChangeEvent& da);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CDailyLimitChangeEvent::CDailyLimitChangeEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CDailyLimitChangeEvent::CDailyLimitChangeEvent(const CDailyLimitChangeEvent& da) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(da);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CDailyLimitChangeEvent::~CDailyLimitChangeEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDailyLimitChangeEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDailyLimitChangeEvent::initialize(void) {
    CLogEntry::initialize();

    dailyLimit = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDailyLimitChangeEvent::duplicate(const CDailyLimitChangeEvent& da) {
    clear();
    CLogEntry::duplicate(da);

    dailyLimit = da.dailyLimit;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CDailyLimitChangeEvent& CDailyLimitChangeEvent::operator=(const CDailyLimitChangeEvent& da) {
    duplicate(da);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CDailyLimitChangeEvent::operator==(const CDailyLimitChangeEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CDailyLimitChangeEvent& v1, const CDailyLimitChangeEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CDailyLimitChangeEvent> CDailyLimitChangeEventArray;
extern CArchive& operator>>(CArchive& archive, CDailyLimitChangeEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CDailyLimitChangeEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

