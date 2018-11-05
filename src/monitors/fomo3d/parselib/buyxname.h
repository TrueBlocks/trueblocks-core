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
class CBuyXname : public CTransaction {
public:
    string_q _affCode;
    biguint_t _team;

public:
    CBuyXname(void);
    CBuyXname(const CBuyXname& bu);
    virtual ~CBuyXname(void);
    CBuyXname& operator=(const CBuyXname& bu);

    DECLARE_NODE(CBuyXname);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CBuyXname& item) const;
    bool operator!=(const CBuyXname& item) const { return !operator==(item); }
    friend bool operator<(const CBuyXname& v1, const CBuyXname& v2);
    friend ostream& operator<<(ostream& os, const CBuyXname& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CBuyXname& bu);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CBuyXname::CBuyXname(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CBuyXname::CBuyXname(const CBuyXname& bu) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(bu);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CBuyXname::~CBuyXname(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBuyXname::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBuyXname::initialize(void) {
    CTransaction::initialize();

    _affCode = "";
    _team = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBuyXname::duplicate(const CBuyXname& bu) {
    clear();
    CTransaction::duplicate(bu);

    _affCode = bu._affCode;
    _team = bu._team;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CBuyXname& CBuyXname::operator=(const CBuyXname& bu) {
    duplicate(bu);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CBuyXname::operator==(const CBuyXname& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CBuyXname& v1, const CBuyXname& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CBuyXname> CBuyXnameArray;
extern CArchive& operator>>(CArchive& archive, CBuyXnameArray& array);
extern CArchive& operator<<(CArchive& archive, const CBuyXnameArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

