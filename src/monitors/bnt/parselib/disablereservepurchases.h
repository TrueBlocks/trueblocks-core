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
class CDisableReservePurchases : public CTransaction {
public:
    address_t _reserveToken;
    bool _disable;

public:
    CDisableReservePurchases(void);
    CDisableReservePurchases(const CDisableReservePurchases& di);
    virtual ~CDisableReservePurchases(void);
    CDisableReservePurchases& operator=(const CDisableReservePurchases& di);

    DECLARE_NODE(CDisableReservePurchases);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CDisableReservePurchases& item) const;
    bool operator!=(const CDisableReservePurchases& item) const { return !operator==(item); }
    friend bool operator<(const CDisableReservePurchases& v1, const CDisableReservePurchases& v2);
    friend ostream& operator<<(ostream& os, const CDisableReservePurchases& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CDisableReservePurchases& di);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CDisableReservePurchases::CDisableReservePurchases(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CDisableReservePurchases::CDisableReservePurchases(const CDisableReservePurchases& di) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(di);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CDisableReservePurchases::~CDisableReservePurchases(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDisableReservePurchases::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDisableReservePurchases::initialize(void) {
    CTransaction::initialize();

    _reserveToken = "";
    _disable = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDisableReservePurchases::duplicate(const CDisableReservePurchases& di) {
    clear();
    CTransaction::duplicate(di);

    _reserveToken = di._reserveToken;
    _disable = di._disable;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CDisableReservePurchases& CDisableReservePurchases::operator=(const CDisableReservePurchases& di) {
    duplicate(di);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CDisableReservePurchases::operator==(const CDisableReservePurchases& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CDisableReservePurchases& v1, const CDisableReservePurchases& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CDisableReservePurchases> CDisableReservePurchasesArray;
extern CArchive& operator>>(CArchive& archive, CDisableReservePurchasesArray& array);
extern CArchive& operator<<(CArchive& archive, const CDisableReservePurchasesArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

