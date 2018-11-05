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
class CReclaimMisplacedTokens : public CTransaction {
public:
    address_t _account;

public:
    CReclaimMisplacedTokens(void);
    CReclaimMisplacedTokens(const CReclaimMisplacedTokens& re);
    virtual ~CReclaimMisplacedTokens(void);
    CReclaimMisplacedTokens& operator=(const CReclaimMisplacedTokens& re);

    DECLARE_NODE(CReclaimMisplacedTokens);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CReclaimMisplacedTokens& item) const;
    bool operator!=(const CReclaimMisplacedTokens& item) const { return !operator==(item); }
    friend bool operator<(const CReclaimMisplacedTokens& v1, const CReclaimMisplacedTokens& v2);
    friend ostream& operator<<(ostream& os, const CReclaimMisplacedTokens& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CReclaimMisplacedTokens& re);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CReclaimMisplacedTokens::CReclaimMisplacedTokens(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CReclaimMisplacedTokens::CReclaimMisplacedTokens(const CReclaimMisplacedTokens& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CReclaimMisplacedTokens::~CReclaimMisplacedTokens(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReclaimMisplacedTokens::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReclaimMisplacedTokens::initialize(void) {
    CTransaction::initialize();

    _account = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReclaimMisplacedTokens::duplicate(const CReclaimMisplacedTokens& re) {
    clear();
    CTransaction::duplicate(re);

    _account = re._account;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CReclaimMisplacedTokens& CReclaimMisplacedTokens::operator=(const CReclaimMisplacedTokens& re) {
    duplicate(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CReclaimMisplacedTokens::operator==(const CReclaimMisplacedTokens& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CReclaimMisplacedTokens& v1, const CReclaimMisplacedTokens& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CReclaimMisplacedTokens> CReclaimMisplacedTokensArray;
extern CArchive& operator>>(CArchive& archive, CReclaimMisplacedTokensArray& array);
extern CArchive& operator<<(CArchive& archive, const CReclaimMisplacedTokensArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

