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
class CProxyPayment : public CTransaction {
public:
    address_t param_0;

public:
    CProxyPayment(void);
    CProxyPayment(const CProxyPayment& pr);
    virtual ~CProxyPayment(void);
    CProxyPayment& operator=(const CProxyPayment& pr);

    DECLARE_NODE(CProxyPayment);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CProxyPayment& item) const;
    bool operator!=(const CProxyPayment& item) const { return !operator==(item); }
    friend bool operator<(const CProxyPayment& v1, const CProxyPayment& v2);
    friend ostream& operator<<(ostream& os, const CProxyPayment& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CProxyPayment& pr);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CProxyPayment::CProxyPayment(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CProxyPayment::CProxyPayment(const CProxyPayment& pr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(pr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CProxyPayment::~CProxyPayment(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CProxyPayment::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CProxyPayment::initialize(void) {
    CTransaction::initialize();

    param_0 = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CProxyPayment::duplicate(const CProxyPayment& pr) {
    clear();
    CTransaction::duplicate(pr);

    param_0 = pr.param_0;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CProxyPayment& CProxyPayment::operator=(const CProxyPayment& pr) {
    duplicate(pr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CProxyPayment::operator==(const CProxyPayment& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CProxyPayment& v1, const CProxyPayment& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CProxyPayment> CProxyPaymentArray;
extern CArchive& operator>>(CArchive& archive, CProxyPaymentArray& array);
extern CArchive& operator<<(CArchive& archive, const CProxyPaymentArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

