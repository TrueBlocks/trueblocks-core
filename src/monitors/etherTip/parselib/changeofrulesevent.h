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
class CChangeOfRulesEvent : public CLogEntry {
public:
    biguint_t minimumQuorum;
    biguint_t debatingPeriodInMinutes;
    bigint_t majorityMargin;

public:
    CChangeOfRulesEvent(void);
    CChangeOfRulesEvent(const CChangeOfRulesEvent& ch);
    virtual ~CChangeOfRulesEvent(void);
    CChangeOfRulesEvent& operator=(const CChangeOfRulesEvent& ch);

    DECLARE_NODE(CChangeOfRulesEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CChangeOfRulesEvent& item) const;
    bool operator!=(const CChangeOfRulesEvent& item) const { return !operator==(item); }
    friend bool operator<(const CChangeOfRulesEvent& v1, const CChangeOfRulesEvent& v2);
    friend ostream& operator<<(ostream& os, const CChangeOfRulesEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CChangeOfRulesEvent& ch);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CChangeOfRulesEvent::CChangeOfRulesEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CChangeOfRulesEvent::CChangeOfRulesEvent(const CChangeOfRulesEvent& ch) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ch);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CChangeOfRulesEvent::~CChangeOfRulesEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeOfRulesEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeOfRulesEvent::initialize(void) {
    CLogEntry::initialize();

    minimumQuorum = 0;
    debatingPeriodInMinutes = 0;
    majorityMargin = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeOfRulesEvent::duplicate(const CChangeOfRulesEvent& ch) {
    clear();
    CLogEntry::duplicate(ch);

    minimumQuorum = ch.minimumQuorum;
    debatingPeriodInMinutes = ch.debatingPeriodInMinutes;
    majorityMargin = ch.majorityMargin;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CChangeOfRulesEvent& CChangeOfRulesEvent::operator=(const CChangeOfRulesEvent& ch) {
    duplicate(ch);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CChangeOfRulesEvent::operator==(const CChangeOfRulesEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CChangeOfRulesEvent& v1, const CChangeOfRulesEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CChangeOfRulesEvent> CChangeOfRulesEventArray;
extern CArchive& operator>>(CArchive& archive, CChangeOfRulesEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CChangeOfRulesEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

