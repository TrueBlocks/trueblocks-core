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
class CBurnTokens : public CTransaction {
public:
    biguint_t _amount;

public:
    CBurnTokens(void);
    CBurnTokens(const CBurnTokens& bu);
    virtual ~CBurnTokens(void);
    CBurnTokens& operator=(const CBurnTokens& bu);

    DECLARE_NODE(CBurnTokens);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CBurnTokens& item) const;
    bool operator!=(const CBurnTokens& item) const { return !operator==(item); }
    friend bool operator<(const CBurnTokens& v1, const CBurnTokens& v2);
    friend ostream& operator<<(ostream& os, const CBurnTokens& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CBurnTokens& bu);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CBurnTokens::CBurnTokens(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CBurnTokens::CBurnTokens(const CBurnTokens& bu) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(bu);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CBurnTokens::~CBurnTokens(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBurnTokens::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBurnTokens::initialize(void) {
    CTransaction::initialize();

    _amount = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBurnTokens::duplicate(const CBurnTokens& bu) {
    clear();
    CTransaction::duplicate(bu);

    _amount = bu._amount;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CBurnTokens& CBurnTokens::operator=(const CBurnTokens& bu) {
    duplicate(bu);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CBurnTokens::operator==(const CBurnTokens& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CBurnTokens& v1, const CBurnTokens& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CBurnTokens> CBurnTokensArray;
extern CArchive& operator>>(CArchive& archive, CBurnTokensArray& array);
extern CArchive& operator<<(CArchive& archive, const CBurnTokensArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

