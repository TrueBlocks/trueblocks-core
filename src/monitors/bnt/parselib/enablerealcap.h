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
class CEnableRealCap : public CTransaction {
public:
    biguint_t _cap;
    biguint_t _key;

public:
    CEnableRealCap(void);
    CEnableRealCap(const CEnableRealCap& en);
    virtual ~CEnableRealCap(void);
    CEnableRealCap& operator=(const CEnableRealCap& en);

    DECLARE_NODE(CEnableRealCap);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CEnableRealCap& item) const;
    bool operator!=(const CEnableRealCap& item) const { return !operator==(item); }
    friend bool operator<(const CEnableRealCap& v1, const CEnableRealCap& v2);
    friend ostream& operator<<(ostream& os, const CEnableRealCap& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CEnableRealCap& en);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CEnableRealCap::CEnableRealCap(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CEnableRealCap::CEnableRealCap(const CEnableRealCap& en) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(en);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CEnableRealCap::~CEnableRealCap(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CEnableRealCap::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CEnableRealCap::initialize(void) {
    CTransaction::initialize();

    _cap = 0;
    _key = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CEnableRealCap::duplicate(const CEnableRealCap& en) {
    clear();
    CTransaction::duplicate(en);

    _cap = en._cap;
    _key = en._key;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CEnableRealCap& CEnableRealCap::operator=(const CEnableRealCap& en) {
    duplicate(en);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CEnableRealCap::operator==(const CEnableRealCap& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CEnableRealCap& v1, const CEnableRealCap& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CEnableRealCap> CEnableRealCapArray;
extern CArchive& operator>>(CArchive& archive, CEnableRealCapArray& array);
extern CArchive& operator<<(CArchive& archive, const CEnableRealCapArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

