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
class CSetCrowdsale : public CTransaction {
public:
    address_t _crowdSaleAddr;
    biguint_t _amountForSale;

public:
    CSetCrowdsale(void);
    CSetCrowdsale(const CSetCrowdsale& se);
    virtual ~CSetCrowdsale(void);
    CSetCrowdsale& operator=(const CSetCrowdsale& se);

    DECLARE_NODE(CSetCrowdsale);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CSetCrowdsale& item) const;
    bool operator!=(const CSetCrowdsale& item) const { return !operator==(item); }
    friend bool operator<(const CSetCrowdsale& v1, const CSetCrowdsale& v2);
    friend ostream& operator<<(ostream& os, const CSetCrowdsale& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CSetCrowdsale& se);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CSetCrowdsale::CSetCrowdsale(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CSetCrowdsale::CSetCrowdsale(const CSetCrowdsale& se) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(se);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CSetCrowdsale::~CSetCrowdsale(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetCrowdsale::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetCrowdsale::initialize(void) {
    CTransaction::initialize();

    _crowdSaleAddr = "";
    _amountForSale = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetCrowdsale::duplicate(const CSetCrowdsale& se) {
    clear();
    CTransaction::duplicate(se);

    _crowdSaleAddr = se._crowdSaleAddr;
    _amountForSale = se._amountForSale;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CSetCrowdsale& CSetCrowdsale::operator=(const CSetCrowdsale& se) {
    duplicate(se);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CSetCrowdsale::operator==(const CSetCrowdsale& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CSetCrowdsale& v1, const CSetCrowdsale& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CSetCrowdsale> CSetCrowdsaleArray;
extern CArchive& operator>>(CArchive& archive, CSetCrowdsaleArray& array);
extern CArchive& operator<<(CArchive& archive, const CSetCrowdsaleArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

