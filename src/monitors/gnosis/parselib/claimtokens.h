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
class CClaimTokens : public CTransaction {
public:
    address_t receiver;

public:
    CClaimTokens(void);
    CClaimTokens(const CClaimTokens& cl);
    virtual ~CClaimTokens(void);
    CClaimTokens& operator=(const CClaimTokens& cl);

    DECLARE_NODE(CClaimTokens);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CClaimTokens& item) const;
    bool operator!=(const CClaimTokens& item) const { return !operator==(item); }
    friend bool operator<(const CClaimTokens& v1, const CClaimTokens& v2);
    friend ostream& operator<<(ostream& os, const CClaimTokens& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CClaimTokens& cl);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CClaimTokens::CClaimTokens(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CClaimTokens::CClaimTokens(const CClaimTokens& cl) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(cl);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CClaimTokens::~CClaimTokens(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CClaimTokens::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CClaimTokens::initialize(void) {
    CTransaction::initialize();

    receiver = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CClaimTokens::duplicate(const CClaimTokens& cl) {
    clear();
    CTransaction::duplicate(cl);

    receiver = cl.receiver;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CClaimTokens& CClaimTokens::operator=(const CClaimTokens& cl) {
    duplicate(cl);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CClaimTokens::operator==(const CClaimTokens& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CClaimTokens& v1, const CClaimTokens& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CClaimTokens> CClaimTokensArray;
extern CArchive& operator>>(CArchive& archive, CClaimTokensArray& array);
extern CArchive& operator<<(CArchive& archive, const CClaimTokensArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

