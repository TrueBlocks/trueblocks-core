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
class CConfirmTransaction : public CTransaction {
public:
    biguint_t transactionId;

public:
    CConfirmTransaction(void);
    CConfirmTransaction(const CConfirmTransaction& co);
    virtual ~CConfirmTransaction(void);
    CConfirmTransaction& operator=(const CConfirmTransaction& co);

    DECLARE_NODE(CConfirmTransaction);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CConfirmTransaction& item) const;
    bool operator!=(const CConfirmTransaction& item) const { return !operator==(item); }
    friend bool operator<(const CConfirmTransaction& v1, const CConfirmTransaction& v2);
    friend ostream& operator<<(ostream& os, const CConfirmTransaction& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CConfirmTransaction& co);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CConfirmTransaction::CConfirmTransaction(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CConfirmTransaction::CConfirmTransaction(const CConfirmTransaction& co) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(co);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CConfirmTransaction::~CConfirmTransaction(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CConfirmTransaction::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CConfirmTransaction::initialize(void) {
    CTransaction::initialize();

    transactionId = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CConfirmTransaction::duplicate(const CConfirmTransaction& co) {
    clear();
    CTransaction::duplicate(co);

    transactionId = co.transactionId;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CConfirmTransaction& CConfirmTransaction::operator=(const CConfirmTransaction& co) {
    duplicate(co);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CConfirmTransaction::operator==(const CConfirmTransaction& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CConfirmTransaction& v1, const CConfirmTransaction& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CConfirmTransaction> CConfirmTransactionArray;
extern CArchive& operator>>(CArchive& archive, CConfirmTransactionArray& array);
extern CArchive& operator<<(CArchive& archive, const CConfirmTransactionArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

