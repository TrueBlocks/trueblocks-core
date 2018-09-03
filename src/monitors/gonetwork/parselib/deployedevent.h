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
class CDeployedEvent : public CLogEntry {
public:
    biguint_t _total_supply;

public:
    CDeployedEvent(void);
    CDeployedEvent(const CDeployedEvent& de);
    virtual ~CDeployedEvent(void);
    CDeployedEvent& operator=(const CDeployedEvent& de);

    DECLARE_NODE(CDeployedEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CDeployedEvent& item) const;
    bool operator!=(const CDeployedEvent& item) const { return !operator==(item); }
    friend bool operator<(const CDeployedEvent& v1, const CDeployedEvent& v2);
    friend ostream& operator<<(ostream& os, const CDeployedEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CDeployedEvent& de);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CDeployedEvent::CDeployedEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CDeployedEvent::CDeployedEvent(const CDeployedEvent& de) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(de);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CDeployedEvent::~CDeployedEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDeployedEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDeployedEvent::initialize(void) {
    CLogEntry::initialize();

    _total_supply = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDeployedEvent::duplicate(const CDeployedEvent& de) {
    clear();
    CLogEntry::duplicate(de);

    _total_supply = de._total_supply;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CDeployedEvent& CDeployedEvent::operator=(const CDeployedEvent& de) {
    duplicate(de);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CDeployedEvent::operator==(const CDeployedEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CDeployedEvent& v1, const CDeployedEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CDeployedEvent> CDeployedEventArray;
extern CArchive& operator>>(CArchive& archive, CDeployedEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CDeployedEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

