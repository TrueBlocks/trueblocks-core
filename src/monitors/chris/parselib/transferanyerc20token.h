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
class CTransferAnyERC20Token : public CTransaction {
public:
    address_t _tokenAddress;
    biguint_t _tokens;

public:
    CTransferAnyERC20Token(void);
    CTransferAnyERC20Token(const CTransferAnyERC20Token& tr);
    virtual ~CTransferAnyERC20Token(void);
    CTransferAnyERC20Token& operator=(const CTransferAnyERC20Token& tr);

    DECLARE_NODE(CTransferAnyERC20Token);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CTransferAnyERC20Token& item) const;
    bool operator!=(const CTransferAnyERC20Token& item) const { return !operator==(item); }
    friend bool operator<(const CTransferAnyERC20Token& v1, const CTransferAnyERC20Token& v2);
    friend ostream& operator<<(ostream& os, const CTransferAnyERC20Token& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CTransferAnyERC20Token& tr);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTransferAnyERC20Token::CTransferAnyERC20Token(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTransferAnyERC20Token::CTransferAnyERC20Token(const CTransferAnyERC20Token& tr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(tr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTransferAnyERC20Token::~CTransferAnyERC20Token(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTransferAnyERC20Token::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTransferAnyERC20Token::initialize(void) {
    CTransaction::initialize();

    _tokenAddress = "";
    _tokens = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTransferAnyERC20Token::duplicate(const CTransferAnyERC20Token& tr) {
    clear();
    CTransaction::duplicate(tr);

    _tokenAddress = tr._tokenAddress;
    _tokens = tr._tokens;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CTransferAnyERC20Token& CTransferAnyERC20Token::operator=(const CTransferAnyERC20Token& tr) {
    duplicate(tr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CTransferAnyERC20Token::operator==(const CTransferAnyERC20Token& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CTransferAnyERC20Token& v1, const CTransferAnyERC20Token& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CTransferAnyERC20Token> CTransferAnyERC20TokenArray;
extern CArchive& operator>>(CArchive& archive, CTransferAnyERC20TokenArray& array);
extern CArchive& operator<<(CArchive& archive, const CTransferAnyERC20TokenArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

