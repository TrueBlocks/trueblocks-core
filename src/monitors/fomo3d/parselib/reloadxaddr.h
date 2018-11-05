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
class CReLoadXaddr : public CTransaction {
public:
    address_t _affCode;
    biguint_t _team;
    biguint_t _eth;

public:
    CReLoadXaddr(void);
    CReLoadXaddr(const CReLoadXaddr& re);
    virtual ~CReLoadXaddr(void);
    CReLoadXaddr& operator=(const CReLoadXaddr& re);

    DECLARE_NODE(CReLoadXaddr);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CReLoadXaddr& item) const;
    bool operator!=(const CReLoadXaddr& item) const { return !operator==(item); }
    friend bool operator<(const CReLoadXaddr& v1, const CReLoadXaddr& v2);
    friend ostream& operator<<(ostream& os, const CReLoadXaddr& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CReLoadXaddr& re);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CReLoadXaddr::CReLoadXaddr(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CReLoadXaddr::CReLoadXaddr(const CReLoadXaddr& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CReLoadXaddr::~CReLoadXaddr(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReLoadXaddr::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReLoadXaddr::initialize(void) {
    CTransaction::initialize();

    _affCode = "";
    _team = 0;
    _eth = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReLoadXaddr::duplicate(const CReLoadXaddr& re) {
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
inline CReLoadXaddr& CReLoadXaddr::operator=(const CReLoadXaddr& re) {
    duplicate(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CReLoadXaddr::operator==(const CReLoadXaddr& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CReLoadXaddr& v1, const CReLoadXaddr& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CReLoadXaddr> CReLoadXaddrArray;
extern CArchive& operator>>(CArchive& archive, CReLoadXaddrArray& array);
extern CArchive& operator<<(CArchive& archive, const CReLoadXaddrArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

