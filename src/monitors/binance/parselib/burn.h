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
class CBurn : public CTransaction {
public:
    biguint_t _value;

public:
    CBurn(void);
    CBurn(const CBurn& bu);
    virtual ~CBurn(void);
    CBurn& operator=(const CBurn& bu);

    DECLARE_NODE(CBurn);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CBurn& item) const;
    bool operator!=(const CBurn& item) const { return !operator==(item); }
    friend bool operator<(const CBurn& v1, const CBurn& v2);
    friend ostream& operator<<(ostream& os, const CBurn& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CBurn& bu);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CBurn::CBurn(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CBurn::CBurn(const CBurn& bu) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(bu);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CBurn::~CBurn(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBurn::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBurn::initialize(void) {
    CTransaction::initialize();

    _value = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBurn::duplicate(const CBurn& bu) {
    clear();
    CTransaction::duplicate(bu);

    _value = bu._value;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CBurn& CBurn::operator=(const CBurn& bu) {
    duplicate(bu);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CBurn::operator==(const CBurn& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CBurn& v1, const CBurn& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CBurn> CBurnArray;
extern CArchive& operator>>(CArchive& archive, CBurnArray& array);
extern CArchive& operator<<(CArchive& archive, const CBurnArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

