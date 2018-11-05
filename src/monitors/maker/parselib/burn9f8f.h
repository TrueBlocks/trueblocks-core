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
class CBurn9f8f : public CTransaction {
public:
    address_t guy;
    biguint_t wad;

public:
    CBurn9f8f(void);
    CBurn9f8f(const CBurn9f8f& bu);
    virtual ~CBurn9f8f(void);
    CBurn9f8f& operator=(const CBurn9f8f& bu);

    DECLARE_NODE(CBurn9f8f);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CBurn9f8f& item) const;
    bool operator!=(const CBurn9f8f& item) const { return !operator==(item); }
    friend bool operator<(const CBurn9f8f& v1, const CBurn9f8f& v2);
    friend ostream& operator<<(ostream& os, const CBurn9f8f& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CBurn9f8f& bu);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CBurn9f8f::CBurn9f8f(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CBurn9f8f::CBurn9f8f(const CBurn9f8f& bu) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(bu);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CBurn9f8f::~CBurn9f8f(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBurn9f8f::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBurn9f8f::initialize(void) {
    CTransaction::initialize();

    guy = "";
    wad = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBurn9f8f::duplicate(const CBurn9f8f& bu) {
    clear();
    CTransaction::duplicate(bu);

    guy = bu.guy;
    wad = bu.wad;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CBurn9f8f& CBurn9f8f::operator=(const CBurn9f8f& bu) {
    duplicate(bu);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CBurn9f8f::operator==(const CBurn9f8f& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CBurn9f8f& v1, const CBurn9f8f& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CBurn9f8f> CBurn9f8fArray;
extern CArchive& operator>>(CArchive& archive, CBurn9f8fArray& array);
extern CArchive& operator<<(CArchive& archive, const CBurn9f8fArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

