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
class CAcceptExistingAskRequestAndBUY : public CTransaction {
public:
    biguint_t _key;
    biguint_t _tokenAmount;
    biguint_t _dollarPrice;

public:
    CAcceptExistingAskRequestAndBUY(void);
    CAcceptExistingAskRequestAndBUY(const CAcceptExistingAskRequestAndBUY& ac);
    virtual ~CAcceptExistingAskRequestAndBUY(void);
    CAcceptExistingAskRequestAndBUY& operator=(const CAcceptExistingAskRequestAndBUY& ac);

    DECLARE_NODE(CAcceptExistingAskRequestAndBUY);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CAcceptExistingAskRequestAndBUY& item) const;
    bool operator!=(const CAcceptExistingAskRequestAndBUY& item) const { return !operator==(item); }
    friend bool operator<(const CAcceptExistingAskRequestAndBUY& v1, const CAcceptExistingAskRequestAndBUY& v2);
    friend ostream& operator<<(ostream& os, const CAcceptExistingAskRequestAndBUY& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CAcceptExistingAskRequestAndBUY& ac);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAcceptExistingAskRequestAndBUY::CAcceptExistingAskRequestAndBUY(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAcceptExistingAskRequestAndBUY::CAcceptExistingAskRequestAndBUY(const CAcceptExistingAskRequestAndBUY& ac) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ac);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAcceptExistingAskRequestAndBUY::~CAcceptExistingAskRequestAndBUY(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAcceptExistingAskRequestAndBUY::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAcceptExistingAskRequestAndBUY::initialize(void) {
    CTransaction::initialize();

    _key = 0;
    _tokenAmount = 0;
    _dollarPrice = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAcceptExistingAskRequestAndBUY::duplicate(const CAcceptExistingAskRequestAndBUY& ac) {
    clear();
    CTransaction::duplicate(ac);

    _key = ac._key;
    _tokenAmount = ac._tokenAmount;
    _dollarPrice = ac._dollarPrice;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CAcceptExistingAskRequestAndBUY& CAcceptExistingAskRequestAndBUY::operator=(const CAcceptExistingAskRequestAndBUY& ac) {
    duplicate(ac);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CAcceptExistingAskRequestAndBUY::operator==(const CAcceptExistingAskRequestAndBUY& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CAcceptExistingAskRequestAndBUY& v1, const CAcceptExistingAskRequestAndBUY& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CAcceptExistingAskRequestAndBUY> CAcceptExistingAskRequestAndBUYArray;
extern CArchive& operator>>(CArchive& archive, CAcceptExistingAskRequestAndBUYArray& array);
extern CArchive& operator<<(CArchive& archive, const CAcceptExistingAskRequestAndBUYArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

