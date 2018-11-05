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
class COwnerSafeWithdrawal : public CTransaction {
public:
    COwnerSafeWithdrawal(void);
    COwnerSafeWithdrawal(const COwnerSafeWithdrawal& ow);
    virtual ~COwnerSafeWithdrawal(void);
    COwnerSafeWithdrawal& operator=(const COwnerSafeWithdrawal& ow);

    DECLARE_NODE(COwnerSafeWithdrawal);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const COwnerSafeWithdrawal& item) const;
    bool operator!=(const COwnerSafeWithdrawal& item) const { return !operator==(item); }
    friend bool operator<(const COwnerSafeWithdrawal& v1, const COwnerSafeWithdrawal& v2);
    friend ostream& operator<<(ostream& os, const COwnerSafeWithdrawal& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const COwnerSafeWithdrawal& ow);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline COwnerSafeWithdrawal::COwnerSafeWithdrawal(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline COwnerSafeWithdrawal::COwnerSafeWithdrawal(const COwnerSafeWithdrawal& ow) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ow);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline COwnerSafeWithdrawal::~COwnerSafeWithdrawal(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COwnerSafeWithdrawal::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COwnerSafeWithdrawal::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COwnerSafeWithdrawal::duplicate(const COwnerSafeWithdrawal& ow) {
    clear();
    CTransaction::duplicate(ow);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline COwnerSafeWithdrawal& COwnerSafeWithdrawal::operator=(const COwnerSafeWithdrawal& ow) {
    duplicate(ow);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool COwnerSafeWithdrawal::operator==(const COwnerSafeWithdrawal& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const COwnerSafeWithdrawal& v1, const COwnerSafeWithdrawal& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<COwnerSafeWithdrawal> COwnerSafeWithdrawalArray;
extern CArchive& operator>>(CArchive& archive, COwnerSafeWithdrawalArray& array);
extern CArchive& operator<<(CArchive& archive, const COwnerSafeWithdrawalArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

