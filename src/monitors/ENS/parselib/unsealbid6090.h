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
class CUnsealBid6090 : public CTransaction {
public:
    string_q _hash;
    biguint_t _value;
    string_q _salt;

public:
    CUnsealBid6090(void);
    CUnsealBid6090(const CUnsealBid6090& un);
    virtual ~CUnsealBid6090(void);
    CUnsealBid6090& operator=(const CUnsealBid6090& un);

    DECLARE_NODE(CUnsealBid6090);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CUnsealBid6090& item) const;
    bool operator!=(const CUnsealBid6090& item) const { return !operator==(item); }
    friend bool operator<(const CUnsealBid6090& v1, const CUnsealBid6090& v2);
    friend ostream& operator<<(ostream& os, const CUnsealBid6090& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CUnsealBid6090& un);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CUnsealBid6090::CUnsealBid6090(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CUnsealBid6090::CUnsealBid6090(const CUnsealBid6090& un) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(un);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CUnsealBid6090::~CUnsealBid6090(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CUnsealBid6090::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CUnsealBid6090::initialize(void) {
    CTransaction::initialize();

    _hash = "";
    _value = 0;
    _salt = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CUnsealBid6090::duplicate(const CUnsealBid6090& un) {
    clear();
    CTransaction::duplicate(un);

    _hash = un._hash;
    _value = un._value;
    _salt = un._salt;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CUnsealBid6090& CUnsealBid6090::operator=(const CUnsealBid6090& un) {
    duplicate(un);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CUnsealBid6090::operator==(const CUnsealBid6090& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CUnsealBid6090& v1, const CUnsealBid6090& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CUnsealBid6090> CUnsealBid6090Array;
extern CArchive& operator>>(CArchive& archive, CUnsealBid6090Array& array);
extern CArchive& operator<<(CArchive& archive, const CUnsealBid6090Array& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

