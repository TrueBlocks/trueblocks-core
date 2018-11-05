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
class CWithdrawRevenue : public CTransaction {
public:
    CWithdrawRevenue(void);
    CWithdrawRevenue(const CWithdrawRevenue& wi);
    virtual ~CWithdrawRevenue(void);
    CWithdrawRevenue& operator=(const CWithdrawRevenue& wi);

    DECLARE_NODE(CWithdrawRevenue);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CWithdrawRevenue& item) const;
    bool operator!=(const CWithdrawRevenue& item) const { return !operator==(item); }
    friend bool operator<(const CWithdrawRevenue& v1, const CWithdrawRevenue& v2);
    friend ostream& operator<<(ostream& os, const CWithdrawRevenue& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CWithdrawRevenue& wi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CWithdrawRevenue::CWithdrawRevenue(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CWithdrawRevenue::CWithdrawRevenue(const CWithdrawRevenue& wi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(wi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CWithdrawRevenue::~CWithdrawRevenue(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CWithdrawRevenue::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CWithdrawRevenue::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CWithdrawRevenue::duplicate(const CWithdrawRevenue& wi) {
    clear();
    CTransaction::duplicate(wi);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CWithdrawRevenue& CWithdrawRevenue::operator=(const CWithdrawRevenue& wi) {
    duplicate(wi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CWithdrawRevenue::operator==(const CWithdrawRevenue& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CWithdrawRevenue& v1, const CWithdrawRevenue& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CWithdrawRevenue> CWithdrawRevenueArray;
extern CArchive& operator>>(CArchive& archive, CWithdrawRevenueArray& array);
extern CArchive& operator<<(CArchive& archive, const CWithdrawRevenueArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

