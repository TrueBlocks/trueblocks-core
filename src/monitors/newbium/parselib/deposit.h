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
class CDeposit : public CTransaction {
public:
    CDeposit(void);
    CDeposit(const CDeposit& de);
    virtual ~CDeposit(void);
    CDeposit& operator=(const CDeposit& de);

    DECLARE_NODE(CDeposit);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CDeposit& item) const;
    bool operator!=(const CDeposit& item) const { return !operator==(item); }
    friend bool operator<(const CDeposit& v1, const CDeposit& v2);
    friend ostream& operator<<(ostream& os, const CDeposit& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CDeposit& de);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CDeposit::CDeposit(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CDeposit::CDeposit(const CDeposit& de) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(de);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CDeposit::~CDeposit(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDeposit::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDeposit::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDeposit::duplicate(const CDeposit& de) {
    clear();
    CTransaction::duplicate(de);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CDeposit& CDeposit::operator=(const CDeposit& de) {
    duplicate(de);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CDeposit::operator==(const CDeposit& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CDeposit& v1, const CDeposit& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CDeposit> CDepositArray;
extern CArchive& operator>>(CArchive& archive, CDepositArray& array);
extern CArchive& operator<<(CArchive& archive, const CDepositArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

