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
class CUnsealBid0122 : public CTransaction {
public:
    string_q _hash;
    address_t _owner;
    biguint_t _value;
    string_q _salt;

public:
    CUnsealBid0122(void);
    CUnsealBid0122(const CUnsealBid0122& un);
    virtual ~CUnsealBid0122(void);
    CUnsealBid0122& operator=(const CUnsealBid0122& un);

    DECLARE_NODE(CUnsealBid0122);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CUnsealBid0122& item) const;
    bool operator!=(const CUnsealBid0122& item) const { return !operator==(item); }
    friend bool operator<(const CUnsealBid0122& v1, const CUnsealBid0122& v2);
    friend ostream& operator<<(ostream& os, const CUnsealBid0122& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CUnsealBid0122& un);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CUnsealBid0122::CUnsealBid0122(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CUnsealBid0122::CUnsealBid0122(const CUnsealBid0122& un) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(un);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CUnsealBid0122::~CUnsealBid0122(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CUnsealBid0122::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CUnsealBid0122::initialize(void) {
    CTransaction::initialize();

    _hash = "";
    _owner = "";
    _value = 0;
    _salt = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CUnsealBid0122::duplicate(const CUnsealBid0122& un) {
    clear();
    CTransaction::duplicate(un);

    _hash = un._hash;
    _owner = un._owner;
    _value = un._value;
    _salt = un._salt;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CUnsealBid0122& CUnsealBid0122::operator=(const CUnsealBid0122& un) {
    duplicate(un);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CUnsealBid0122::operator==(const CUnsealBid0122& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CUnsealBid0122& v1, const CUnsealBid0122& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CUnsealBid0122> CUnsealBid0122Array;
extern CArchive& operator>>(CArchive& archive, CUnsealBid0122Array& array);
extern CArchive& operator<<(CArchive& archive, const CUnsealBid0122Array& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

