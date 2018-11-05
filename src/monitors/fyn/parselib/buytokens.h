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
class CBuyTokens : public CTransaction {
public:
    address_t _beneficiary;

public:
    CBuyTokens(void);
    CBuyTokens(const CBuyTokens& bu);
    virtual ~CBuyTokens(void);
    CBuyTokens& operator=(const CBuyTokens& bu);

    DECLARE_NODE(CBuyTokens);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CBuyTokens& item) const;
    bool operator!=(const CBuyTokens& item) const { return !operator==(item); }
    friend bool operator<(const CBuyTokens& v1, const CBuyTokens& v2);
    friend ostream& operator<<(ostream& os, const CBuyTokens& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CBuyTokens& bu);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CBuyTokens::CBuyTokens(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CBuyTokens::CBuyTokens(const CBuyTokens& bu) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(bu);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CBuyTokens::~CBuyTokens(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBuyTokens::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBuyTokens::initialize(void) {
    CTransaction::initialize();

    _beneficiary = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBuyTokens::duplicate(const CBuyTokens& bu) {
    clear();
    CTransaction::duplicate(bu);

    _beneficiary = bu._beneficiary;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CBuyTokens& CBuyTokens::operator=(const CBuyTokens& bu) {
    duplicate(bu);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CBuyTokens::operator==(const CBuyTokens& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CBuyTokens& v1, const CBuyTokens& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CBuyTokens> CBuyTokensArray;
extern CArchive& operator>>(CArchive& archive, CBuyTokensArray& array);
extern CArchive& operator<<(CArchive& archive, const CBuyTokensArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

