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
class CCalcCurrentTokenPrice : public CTransaction {
public:
    CCalcCurrentTokenPrice(void);
    CCalcCurrentTokenPrice(const CCalcCurrentTokenPrice& ca);
    virtual ~CCalcCurrentTokenPrice(void);
    CCalcCurrentTokenPrice& operator=(const CCalcCurrentTokenPrice& ca);

    DECLARE_NODE(CCalcCurrentTokenPrice);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CCalcCurrentTokenPrice& item) const;
    bool operator!=(const CCalcCurrentTokenPrice& item) const { return !operator==(item); }
    friend bool operator<(const CCalcCurrentTokenPrice& v1, const CCalcCurrentTokenPrice& v2);
    friend ostream& operator<<(ostream& os, const CCalcCurrentTokenPrice& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CCalcCurrentTokenPrice& ca);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CCalcCurrentTokenPrice::CCalcCurrentTokenPrice(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CCalcCurrentTokenPrice::CCalcCurrentTokenPrice(const CCalcCurrentTokenPrice& ca) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ca);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CCalcCurrentTokenPrice::~CCalcCurrentTokenPrice(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCalcCurrentTokenPrice::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCalcCurrentTokenPrice::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCalcCurrentTokenPrice::duplicate(const CCalcCurrentTokenPrice& ca) {
    clear();
    CTransaction::duplicate(ca);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CCalcCurrentTokenPrice& CCalcCurrentTokenPrice::operator=(const CCalcCurrentTokenPrice& ca) {
    duplicate(ca);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CCalcCurrentTokenPrice::operator==(const CCalcCurrentTokenPrice& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CCalcCurrentTokenPrice& v1, const CCalcCurrentTokenPrice& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CCalcCurrentTokenPrice> CCalcCurrentTokenPriceArray;
extern CArchive& operator>>(CArchive& archive, CCalcCurrentTokenPriceArray& array);
extern CArchive& operator<<(CArchive& archive, const CCalcCurrentTokenPriceArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

