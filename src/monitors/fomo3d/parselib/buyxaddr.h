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
class CBuyXaddr : public CTransaction {
public:
    address_t _affCode;
    biguint_t _team;

public:
    CBuyXaddr(void);
    CBuyXaddr(const CBuyXaddr& bu);
    virtual ~CBuyXaddr(void);
    CBuyXaddr& operator=(const CBuyXaddr& bu);

    DECLARE_NODE(CBuyXaddr);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CBuyXaddr& item) const;
    bool operator!=(const CBuyXaddr& item) const { return !operator==(item); }
    friend bool operator<(const CBuyXaddr& v1, const CBuyXaddr& v2);
    friend ostream& operator<<(ostream& os, const CBuyXaddr& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CBuyXaddr& bu);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CBuyXaddr::CBuyXaddr(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CBuyXaddr::CBuyXaddr(const CBuyXaddr& bu) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(bu);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CBuyXaddr::~CBuyXaddr(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBuyXaddr::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBuyXaddr::initialize(void) {
    CTransaction::initialize();

    _affCode = "";
    _team = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBuyXaddr::duplicate(const CBuyXaddr& bu) {
    clear();
    CTransaction::duplicate(bu);

    _affCode = bu._affCode;
    _team = bu._team;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CBuyXaddr& CBuyXaddr::operator=(const CBuyXaddr& bu) {
    duplicate(bu);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CBuyXaddr::operator==(const CBuyXaddr& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CBuyXaddr& v1, const CBuyXaddr& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CBuyXaddr> CBuyXaddrArray;
extern CArchive& operator>>(CArchive& archive, CBuyXaddrArray& array);
extern CArchive& operator<<(CArchive& archive, const CBuyXaddrArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

