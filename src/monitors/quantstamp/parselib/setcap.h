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
class CSetCap : public CTransaction {
public:
    biguint_t _cap;

public:
    CSetCap(void);
    CSetCap(const CSetCap& se);
    virtual ~CSetCap(void);
    CSetCap& operator=(const CSetCap& se);

    DECLARE_NODE(CSetCap);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CSetCap& item) const;
    bool operator!=(const CSetCap& item) const { return !operator==(item); }
    friend bool operator<(const CSetCap& v1, const CSetCap& v2);
    friend ostream& operator<<(ostream& os, const CSetCap& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CSetCap& se);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CSetCap::CSetCap(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CSetCap::CSetCap(const CSetCap& se) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(se);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CSetCap::~CSetCap(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetCap::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetCap::initialize(void) {
    CTransaction::initialize();

    _cap = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetCap::duplicate(const CSetCap& se) {
    clear();
    CTransaction::duplicate(se);

    _cap = se._cap;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CSetCap& CSetCap::operator=(const CSetCap& se) {
    duplicate(se);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CSetCap::operator==(const CSetCap& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CSetCap& v1, const CSetCap& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CSetCap> CSetCapArray;
extern CArchive& operator>>(CArchive& archive, CSetCapArray& array);
extern CArchive& operator<<(CArchive& archive, const CSetCapArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

