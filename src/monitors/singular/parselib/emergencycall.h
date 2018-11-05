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
#include "transaction.h"

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CEmergencyCall : public CTransaction {
public:
    CEmergencyCall(void);
    CEmergencyCall(const CEmergencyCall& em);
    virtual ~CEmergencyCall(void);
    CEmergencyCall& operator=(const CEmergencyCall& em);

    DECLARE_NODE(CEmergencyCall);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CEmergencyCall& item) const;
    bool operator!=(const CEmergencyCall& item) const { return !operator==(item); }
    friend bool operator<(const CEmergencyCall& v1, const CEmergencyCall& v2);
    friend ostream& operator<<(ostream& os, const CEmergencyCall& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CEmergencyCall& em);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CEmergencyCall::CEmergencyCall(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CEmergencyCall::CEmergencyCall(const CEmergencyCall& em) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(em);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CEmergencyCall::~CEmergencyCall(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CEmergencyCall::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CEmergencyCall::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CEmergencyCall::duplicate(const CEmergencyCall& em) {
    clear();
    CTransaction::duplicate(em);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CEmergencyCall& CEmergencyCall::operator=(const CEmergencyCall& em) {
    duplicate(em);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CEmergencyCall::operator==(const CEmergencyCall& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CEmergencyCall& v1, const CEmergencyCall& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CEmergencyCall> CEmergencyCallArray;
extern CArchive& operator>>(CArchive& archive, CEmergencyCallArray& array);
extern CArchive& operator<<(CArchive& archive, const CEmergencyCallArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

