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
class CExecuteTransaction : public CTransaction {
public:
    biguint_t transactionId;

public:
    CExecuteTransaction(void);
    CExecuteTransaction(const CExecuteTransaction& ex);
    virtual ~CExecuteTransaction(void);
    CExecuteTransaction& operator=(const CExecuteTransaction& ex);

    DECLARE_NODE(CExecuteTransaction);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CExecuteTransaction& item) const;
    bool operator!=(const CExecuteTransaction& item) const { return !operator==(item); }
    friend bool operator<(const CExecuteTransaction& v1, const CExecuteTransaction& v2);
    friend ostream& operator<<(ostream& os, const CExecuteTransaction& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CExecuteTransaction& ex);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CExecuteTransaction::CExecuteTransaction(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CExecuteTransaction::CExecuteTransaction(const CExecuteTransaction& ex) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ex);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CExecuteTransaction::~CExecuteTransaction(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CExecuteTransaction::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CExecuteTransaction::initialize(void) {
    CTransaction::initialize();

    transactionId = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CExecuteTransaction::duplicate(const CExecuteTransaction& ex) {
    clear();
    CTransaction::duplicate(ex);

    transactionId = ex.transactionId;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CExecuteTransaction& CExecuteTransaction::operator=(const CExecuteTransaction& ex) {
    duplicate(ex);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CExecuteTransaction::operator==(const CExecuteTransaction& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CExecuteTransaction& v1, const CExecuteTransaction& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CExecuteTransaction> CExecuteTransactionArray;
extern CArchive& operator>>(CArchive& archive, CExecuteTransactionArray& array);
extern CArchive& operator<<(CArchive& archive, const CExecuteTransactionArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

