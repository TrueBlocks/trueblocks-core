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
class CBuyXid : public CTransaction {
public:
    biguint_t _affCode;
    biguint_t _team;

public:
    CBuyXid(void);
    CBuyXid(const CBuyXid& bu);
    virtual ~CBuyXid(void);
    CBuyXid& operator=(const CBuyXid& bu);

    DECLARE_NODE(CBuyXid);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CBuyXid& item) const;
    bool operator!=(const CBuyXid& item) const { return !operator==(item); }
    friend bool operator<(const CBuyXid& v1, const CBuyXid& v2);
    friend ostream& operator<<(ostream& os, const CBuyXid& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CBuyXid& bu);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CBuyXid::CBuyXid(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CBuyXid::CBuyXid(const CBuyXid& bu) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(bu);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CBuyXid::~CBuyXid(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBuyXid::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBuyXid::initialize(void) {
    CTransaction::initialize();

    _affCode = 0;
    _team = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBuyXid::duplicate(const CBuyXid& bu) {
    clear();
    CTransaction::duplicate(bu);

    _affCode = bu._affCode;
    _team = bu._team;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CBuyXid& CBuyXid::operator=(const CBuyXid& bu) {
    duplicate(bu);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CBuyXid::operator==(const CBuyXid& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CBuyXid& v1, const CBuyXid& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CBuyXid> CBuyXidArray;
extern CArchive& operator>>(CArchive& archive, CBuyXidArray& array);
extern CArchive& operator<<(CArchive& archive, const CBuyXidArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

