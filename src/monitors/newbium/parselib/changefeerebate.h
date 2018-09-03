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
class CChangeFeeRebate : public CTransaction {
public:
    biguint_t feeRebate_;

public:
    CChangeFeeRebate(void);
    CChangeFeeRebate(const CChangeFeeRebate& ch);
    virtual ~CChangeFeeRebate(void);
    CChangeFeeRebate& operator=(const CChangeFeeRebate& ch);

    DECLARE_NODE(CChangeFeeRebate);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CChangeFeeRebate& item) const;
    bool operator!=(const CChangeFeeRebate& item) const { return !operator==(item); }
    friend bool operator<(const CChangeFeeRebate& v1, const CChangeFeeRebate& v2);
    friend ostream& operator<<(ostream& os, const CChangeFeeRebate& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CChangeFeeRebate& ch);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CChangeFeeRebate::CChangeFeeRebate(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CChangeFeeRebate::CChangeFeeRebate(const CChangeFeeRebate& ch) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ch);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CChangeFeeRebate::~CChangeFeeRebate(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeFeeRebate::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeFeeRebate::initialize(void) {
    CTransaction::initialize();

    feeRebate_ = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeFeeRebate::duplicate(const CChangeFeeRebate& ch) {
    clear();
    CTransaction::duplicate(ch);

    feeRebate_ = ch.feeRebate_;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CChangeFeeRebate& CChangeFeeRebate::operator=(const CChangeFeeRebate& ch) {
    duplicate(ch);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CChangeFeeRebate::operator==(const CChangeFeeRebate& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CChangeFeeRebate& v1, const CChangeFeeRebate& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CChangeFeeRebate> CChangeFeeRebateArray;
extern CArchive& operator>>(CArchive& archive, CChangeFeeRebateArray& array);
extern CArchive& operator<<(CArchive& archive, const CChangeFeeRebateArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

