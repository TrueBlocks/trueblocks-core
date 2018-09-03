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
class CGenerateTokens : public CTransaction {
public:
    address_t _owner;
    biguint_t _amount;

public:
    CGenerateTokens(void);
    CGenerateTokens(const CGenerateTokens& ge);
    virtual ~CGenerateTokens(void);
    CGenerateTokens& operator=(const CGenerateTokens& ge);

    DECLARE_NODE(CGenerateTokens);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CGenerateTokens& item) const;
    bool operator!=(const CGenerateTokens& item) const { return !operator==(item); }
    friend bool operator<(const CGenerateTokens& v1, const CGenerateTokens& v2);
    friend ostream& operator<<(ostream& os, const CGenerateTokens& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CGenerateTokens& ge);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CGenerateTokens::CGenerateTokens(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CGenerateTokens::CGenerateTokens(const CGenerateTokens& ge) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ge);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CGenerateTokens::~CGenerateTokens(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CGenerateTokens::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CGenerateTokens::initialize(void) {
    CTransaction::initialize();

    _owner = "";
    _amount = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CGenerateTokens::duplicate(const CGenerateTokens& ge) {
    clear();
    CTransaction::duplicate(ge);

    _owner = ge._owner;
    _amount = ge._amount;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CGenerateTokens& CGenerateTokens::operator=(const CGenerateTokens& ge) {
    duplicate(ge);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CGenerateTokens::operator==(const CGenerateTokens& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CGenerateTokens& v1, const CGenerateTokens& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CGenerateTokens> CGenerateTokensArray;
extern CArchive& operator>>(CArchive& archive, CGenerateTokensArray& array);
extern CArchive& operator<<(CArchive& archive, const CGenerateTokensArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

