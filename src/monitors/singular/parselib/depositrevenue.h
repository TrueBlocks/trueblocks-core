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
class CDepositRevenue : public CTransaction {
public:
    CDepositRevenue(void);
    CDepositRevenue(const CDepositRevenue& de);
    virtual ~CDepositRevenue(void);
    CDepositRevenue& operator=(const CDepositRevenue& de);

    DECLARE_NODE(CDepositRevenue);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CDepositRevenue& item) const;
    bool operator!=(const CDepositRevenue& item) const { return !operator==(item); }
    friend bool operator<(const CDepositRevenue& v1, const CDepositRevenue& v2);
    friend ostream& operator<<(ostream& os, const CDepositRevenue& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CDepositRevenue& de);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CDepositRevenue::CDepositRevenue(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CDepositRevenue::CDepositRevenue(const CDepositRevenue& de) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(de);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CDepositRevenue::~CDepositRevenue(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDepositRevenue::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDepositRevenue::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDepositRevenue::duplicate(const CDepositRevenue& de) {
    clear();
    CTransaction::duplicate(de);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CDepositRevenue& CDepositRevenue::operator=(const CDepositRevenue& de) {
    duplicate(de);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CDepositRevenue::operator==(const CDepositRevenue& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CDepositRevenue& v1, const CDepositRevenue& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CDepositRevenue> CDepositRevenueArray;
extern CArchive& operator>>(CArchive& archive, CDepositRevenueArray& array);
extern CArchive& operator<<(CArchive& archive, const CDepositRevenueArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

