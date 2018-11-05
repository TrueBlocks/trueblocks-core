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
class CChangePrice : public CTransaction {
public:
    biguint_t _price;

public:
    CChangePrice(void);
    CChangePrice(const CChangePrice& ch);
    virtual ~CChangePrice(void);
    CChangePrice& operator=(const CChangePrice& ch);

    DECLARE_NODE(CChangePrice);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CChangePrice& item) const;
    bool operator!=(const CChangePrice& item) const { return !operator==(item); }
    friend bool operator<(const CChangePrice& v1, const CChangePrice& v2);
    friend ostream& operator<<(ostream& os, const CChangePrice& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CChangePrice& ch);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CChangePrice::CChangePrice(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CChangePrice::CChangePrice(const CChangePrice& ch) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ch);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CChangePrice::~CChangePrice(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangePrice::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangePrice::initialize(void) {
    CTransaction::initialize();

    _price = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangePrice::duplicate(const CChangePrice& ch) {
    clear();
    CTransaction::duplicate(ch);

    _price = ch._price;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CChangePrice& CChangePrice::operator=(const CChangePrice& ch) {
    duplicate(ch);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CChangePrice::operator==(const CChangePrice& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CChangePrice& v1, const CChangePrice& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CChangePrice> CChangePriceArray;
extern CArchive& operator>>(CArchive& archive, CChangePriceArray& array);
extern CArchive& operator<<(CArchive& archive, const CChangePriceArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

