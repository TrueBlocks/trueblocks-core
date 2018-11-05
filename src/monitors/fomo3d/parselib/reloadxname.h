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
class CReLoadXname : public CTransaction {
public:
    string_q _affCode;
    biguint_t _team;
    biguint_t _eth;

public:
    CReLoadXname(void);
    CReLoadXname(const CReLoadXname& re);
    virtual ~CReLoadXname(void);
    CReLoadXname& operator=(const CReLoadXname& re);

    DECLARE_NODE(CReLoadXname);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CReLoadXname& item) const;
    bool operator!=(const CReLoadXname& item) const { return !operator==(item); }
    friend bool operator<(const CReLoadXname& v1, const CReLoadXname& v2);
    friend ostream& operator<<(ostream& os, const CReLoadXname& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CReLoadXname& re);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CReLoadXname::CReLoadXname(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CReLoadXname::CReLoadXname(const CReLoadXname& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CReLoadXname::~CReLoadXname(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReLoadXname::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReLoadXname::initialize(void) {
    CTransaction::initialize();

    _affCode = "";
    _team = 0;
    _eth = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReLoadXname::duplicate(const CReLoadXname& re) {
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
inline CReLoadXname& CReLoadXname::operator=(const CReLoadXname& re) {
    duplicate(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CReLoadXname::operator==(const CReLoadXname& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CReLoadXname& v1, const CReLoadXname& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CReLoadXname> CReLoadXnameArray;
extern CArchive& operator>>(CArchive& archive, CReLoadXnameArray& array);
extern CArchive& operator<<(CArchive& archive, const CReLoadXnameArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

