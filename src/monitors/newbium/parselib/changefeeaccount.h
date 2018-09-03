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
class CChangeFeeAccount : public CTransaction {
public:
    address_t feeAccount_;

public:
    CChangeFeeAccount(void);
    CChangeFeeAccount(const CChangeFeeAccount& ch);
    virtual ~CChangeFeeAccount(void);
    CChangeFeeAccount& operator=(const CChangeFeeAccount& ch);

    DECLARE_NODE(CChangeFeeAccount);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CChangeFeeAccount& item) const;
    bool operator!=(const CChangeFeeAccount& item) const { return !operator==(item); }
    friend bool operator<(const CChangeFeeAccount& v1, const CChangeFeeAccount& v2);
    friend ostream& operator<<(ostream& os, const CChangeFeeAccount& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CChangeFeeAccount& ch);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CChangeFeeAccount::CChangeFeeAccount(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CChangeFeeAccount::CChangeFeeAccount(const CChangeFeeAccount& ch) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ch);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CChangeFeeAccount::~CChangeFeeAccount(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeFeeAccount::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeFeeAccount::initialize(void) {
    CTransaction::initialize();

    feeAccount_ = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeFeeAccount::duplicate(const CChangeFeeAccount& ch) {
    clear();
    CTransaction::duplicate(ch);

    feeAccount_ = ch.feeAccount_;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CChangeFeeAccount& CChangeFeeAccount::operator=(const CChangeFeeAccount& ch) {
    duplicate(ch);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CChangeFeeAccount::operator==(const CChangeFeeAccount& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CChangeFeeAccount& v1, const CChangeFeeAccount& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CChangeFeeAccount> CChangeFeeAccountArray;
extern CArchive& operator>>(CArchive& archive, CChangeFeeAccountArray& array);
extern CArchive& operator<<(CArchive& archive, const CChangeFeeAccountArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

