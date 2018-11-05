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
class CRequirementChangeEvent : public CLogEntry {
public:
    biguint_t required;

public:
    CRequirementChangeEvent(void);
    CRequirementChangeEvent(const CRequirementChangeEvent& re);
    virtual ~CRequirementChangeEvent(void);
    CRequirementChangeEvent& operator=(const CRequirementChangeEvent& re);

    DECLARE_NODE(CRequirementChangeEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CRequirementChangeEvent& item) const;
    bool operator!=(const CRequirementChangeEvent& item) const { return !operator==(item); }
    friend bool operator<(const CRequirementChangeEvent& v1, const CRequirementChangeEvent& v2);
    friend ostream& operator<<(ostream& os, const CRequirementChangeEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CRequirementChangeEvent& re);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CRequirementChangeEvent::CRequirementChangeEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CRequirementChangeEvent::CRequirementChangeEvent(const CRequirementChangeEvent& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CRequirementChangeEvent::~CRequirementChangeEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRequirementChangeEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRequirementChangeEvent::initialize(void) {
    CLogEntry::initialize();

    required = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRequirementChangeEvent::duplicate(const CRequirementChangeEvent& re) {
    clear();
    CLogEntry::duplicate(re);

    required = re.required;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CRequirementChangeEvent& CRequirementChangeEvent::operator=(const CRequirementChangeEvent& re) {
    duplicate(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CRequirementChangeEvent::operator==(const CRequirementChangeEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CRequirementChangeEvent& v1, const CRequirementChangeEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CRequirementChangeEvent> CRequirementChangeEventArray;
extern CArchive& operator>>(CArchive& archive, CRequirementChangeEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CRequirementChangeEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

