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
class CGrantVestedTokens : public CTransaction {
public:
    address_t _to;
    biguint_t _value;
    uint64_t _start;
    uint64_t _cliff;
    uint64_t _vesting;

public:
    CGrantVestedTokens(void);
    CGrantVestedTokens(const CGrantVestedTokens& gr);
    virtual ~CGrantVestedTokens(void);
    CGrantVestedTokens& operator=(const CGrantVestedTokens& gr);

    DECLARE_NODE(CGrantVestedTokens);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CGrantVestedTokens& item) const;
    bool operator!=(const CGrantVestedTokens& item) const { return !operator==(item); }
    friend bool operator<(const CGrantVestedTokens& v1, const CGrantVestedTokens& v2);
    friend ostream& operator<<(ostream& os, const CGrantVestedTokens& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CGrantVestedTokens& gr);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CGrantVestedTokens::CGrantVestedTokens(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CGrantVestedTokens::CGrantVestedTokens(const CGrantVestedTokens& gr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(gr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CGrantVestedTokens::~CGrantVestedTokens(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CGrantVestedTokens::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CGrantVestedTokens::initialize(void) {
    CTransaction::initialize();

    _to = "";
    _value = 0;
    _start = 0;
    _cliff = 0;
    _vesting = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CGrantVestedTokens::duplicate(const CGrantVestedTokens& gr) {
    clear();
    CTransaction::duplicate(gr);

    _to = gr._to;
    _value = gr._value;
    _start = gr._start;
    _cliff = gr._cliff;
    _vesting = gr._vesting;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CGrantVestedTokens& CGrantVestedTokens::operator=(const CGrantVestedTokens& gr) {
    duplicate(gr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CGrantVestedTokens::operator==(const CGrantVestedTokens& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CGrantVestedTokens& v1, const CGrantVestedTokens& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CGrantVestedTokens> CGrantVestedTokensArray;
extern CArchive& operator>>(CArchive& archive, CGrantVestedTokensArray& array);
extern CArchive& operator<<(CArchive& archive, const CGrantVestedTokensArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

