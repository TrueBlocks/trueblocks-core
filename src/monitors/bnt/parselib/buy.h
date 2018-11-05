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
class CBuy : public CTransaction {
public:
    address_t _reserveToken;
    biguint_t _depositAmount;
    biguint_t _minReturn;

public:
    CBuy(void);
    CBuy(const CBuy& bu);
    virtual ~CBuy(void);
    CBuy& operator=(const CBuy& bu);

    DECLARE_NODE(CBuy);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CBuy& item) const;
    bool operator!=(const CBuy& item) const { return !operator==(item); }
    friend bool operator<(const CBuy& v1, const CBuy& v2);
    friend ostream& operator<<(ostream& os, const CBuy& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CBuy& bu);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CBuy::CBuy(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CBuy::CBuy(const CBuy& bu) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(bu);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CBuy::~CBuy(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBuy::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBuy::initialize(void) {
    CTransaction::initialize();

    _reserveToken = "";
    _depositAmount = 0;
    _minReturn = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBuy::duplicate(const CBuy& bu) {
    clear();
    CTransaction::duplicate(bu);

    _reserveToken = bu._reserveToken;
    _depositAmount = bu._depositAmount;
    _minReturn = bu._minReturn;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CBuy& CBuy::operator=(const CBuy& bu) {
    duplicate(bu);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CBuy::operator==(const CBuy& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CBuy& v1, const CBuy& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CBuy> CBuyArray;
extern CArchive& operator>>(CArchive& archive, CBuyArray& array);
extern CArchive& operator<<(CArchive& archive, const CBuyArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

