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
class CDeployed423bEvent : public CLogEntry {
public:
    biguint_t _price_start;
    biguint_t _price_constant1;
    biguint_t _price_exponent1;
    biguint_t _price_constant2;
    biguint_t _price_exponent2;

public:
    CDeployed423bEvent(void);
    CDeployed423bEvent(const CDeployed423bEvent& de);
    virtual ~CDeployed423bEvent(void);
    CDeployed423bEvent& operator=(const CDeployed423bEvent& de);

    DECLARE_NODE(CDeployed423bEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CDeployed423bEvent& item) const;
    bool operator!=(const CDeployed423bEvent& item) const { return !operator==(item); }
    friend bool operator<(const CDeployed423bEvent& v1, const CDeployed423bEvent& v2);
    friend ostream& operator<<(ostream& os, const CDeployed423bEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CDeployed423bEvent& de);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CDeployed423bEvent::CDeployed423bEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CDeployed423bEvent::CDeployed423bEvent(const CDeployed423bEvent& de) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(de);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CDeployed423bEvent::~CDeployed423bEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDeployed423bEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDeployed423bEvent::initialize(void) {
    CLogEntry::initialize();

    _price_start = 0;
    _price_constant1 = 0;
    _price_exponent1 = 0;
    _price_constant2 = 0;
    _price_exponent2 = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDeployed423bEvent::duplicate(const CDeployed423bEvent& de) {
    clear();
    CLogEntry::duplicate(de);

    _price_start = de._price_start;
    _price_constant1 = de._price_constant1;
    _price_exponent1 = de._price_exponent1;
    _price_constant2 = de._price_constant2;
    _price_exponent2 = de._price_exponent2;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CDeployed423bEvent& CDeployed423bEvent::operator=(const CDeployed423bEvent& de) {
    duplicate(de);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CDeployed423bEvent::operator==(const CDeployed423bEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CDeployed423bEvent& v1, const CDeployed423bEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CDeployed423bEvent> CDeployed423bEventArray;
extern CArchive& operator>>(CArchive& archive, CDeployed423bEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CDeployed423bEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

