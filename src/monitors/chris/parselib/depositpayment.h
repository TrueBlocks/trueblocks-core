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
class CDepositPayment : public CTransaction {
public:
    CDepositPayment(void);
    CDepositPayment(const CDepositPayment& de);
    virtual ~CDepositPayment(void);
    CDepositPayment& operator=(const CDepositPayment& de);

    DECLARE_NODE(CDepositPayment);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CDepositPayment& item) const;
    bool operator!=(const CDepositPayment& item) const { return !operator==(item); }
    friend bool operator<(const CDepositPayment& v1, const CDepositPayment& v2);
    friend ostream& operator<<(ostream& os, const CDepositPayment& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CDepositPayment& de);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CDepositPayment::CDepositPayment(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CDepositPayment::CDepositPayment(const CDepositPayment& de) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(de);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CDepositPayment::~CDepositPayment(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDepositPayment::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDepositPayment::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDepositPayment::duplicate(const CDepositPayment& de) {
    clear();
    CTransaction::duplicate(de);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CDepositPayment& CDepositPayment::operator=(const CDepositPayment& de) {
    duplicate(de);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CDepositPayment::operator==(const CDepositPayment& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CDepositPayment& v1, const CDepositPayment& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CDepositPayment> CDepositPaymentArray;
extern CArchive& operator>>(CArchive& archive, CDepositPaymentArray& array);
extern CArchive& operator<<(CArchive& archive, const CDepositPaymentArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

