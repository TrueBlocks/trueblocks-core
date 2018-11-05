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
class CReLoadXid : public CTransaction {
public:
    biguint_t _affCode;
    biguint_t _team;
    biguint_t _eth;

public:
    CReLoadXid(void);
    CReLoadXid(const CReLoadXid& re);
    virtual ~CReLoadXid(void);
    CReLoadXid& operator=(const CReLoadXid& re);

    DECLARE_NODE(CReLoadXid);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CReLoadXid& item) const;
    bool operator!=(const CReLoadXid& item) const { return !operator==(item); }
    friend bool operator<(const CReLoadXid& v1, const CReLoadXid& v2);
    friend ostream& operator<<(ostream& os, const CReLoadXid& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CReLoadXid& re);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CReLoadXid::CReLoadXid(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CReLoadXid::CReLoadXid(const CReLoadXid& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CReLoadXid::~CReLoadXid(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReLoadXid::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReLoadXid::initialize(void) {
    CTransaction::initialize();

    _affCode = 0;
    _team = 0;
    _eth = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReLoadXid::duplicate(const CReLoadXid& re) {
    clear();
    CTransaction::duplicate(re);

    _affCode = re._affCode;
    _team = re._team;
    _eth = re._eth;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CReLoadXid& CReLoadXid::operator=(const CReLoadXid& re) {
    duplicate(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CReLoadXid::operator==(const CReLoadXid& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CReLoadXid& v1, const CReLoadXid& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CReLoadXid> CReLoadXidArray;
extern CArchive& operator>>(CArchive& archive, CReLoadXidArray& array);
extern CArchive& operator<<(CArchive& archive, const CReLoadXidArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

