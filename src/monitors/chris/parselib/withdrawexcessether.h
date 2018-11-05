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
class CWithdrawExcessEther : public CTransaction {
public:
    CWithdrawExcessEther(void);
    CWithdrawExcessEther(const CWithdrawExcessEther& wi);
    virtual ~CWithdrawExcessEther(void);
    CWithdrawExcessEther& operator=(const CWithdrawExcessEther& wi);

    DECLARE_NODE(CWithdrawExcessEther);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CWithdrawExcessEther& item) const;
    bool operator!=(const CWithdrawExcessEther& item) const { return !operator==(item); }
    friend bool operator<(const CWithdrawExcessEther& v1, const CWithdrawExcessEther& v2);
    friend ostream& operator<<(ostream& os, const CWithdrawExcessEther& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CWithdrawExcessEther& wi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CWithdrawExcessEther::CWithdrawExcessEther(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CWithdrawExcessEther::CWithdrawExcessEther(const CWithdrawExcessEther& wi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(wi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CWithdrawExcessEther::~CWithdrawExcessEther(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CWithdrawExcessEther::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CWithdrawExcessEther::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CWithdrawExcessEther::duplicate(const CWithdrawExcessEther& wi) {
    clear();
    CTransaction::duplicate(wi);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CWithdrawExcessEther& CWithdrawExcessEther::operator=(const CWithdrawExcessEther& wi) {
    duplicate(wi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CWithdrawExcessEther::operator==(const CWithdrawExcessEther& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CWithdrawExcessEther& v1, const CWithdrawExcessEther& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CWithdrawExcessEther> CWithdrawExcessEtherArray;
extern CArchive& operator>>(CArchive& archive, CWithdrawExcessEtherArray& array);
extern CArchive& operator<<(CArchive& archive, const CWithdrawExcessEtherArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

