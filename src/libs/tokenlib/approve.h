#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
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
class QApprove : public CTransaction {
public:
    address_t _spender;
    biguint_t _value;

public:
    QApprove(void);
    QApprove(const QApprove& ap);
    virtual ~QApprove(void);
    QApprove& operator=(const QApprove& ap);

    DECLARE_NODE(QApprove);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const QApprove& item) const;
    bool operator!=(const QApprove& item) const { return !operator==(item); }
    friend bool operator<(const QApprove& v1, const QApprove& v2);
    friend ostream& operator<<(ostream& os, const QApprove& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const QApprove& ap);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QApprove::QApprove(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QApprove::QApprove(const QApprove& ap) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ap);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QApprove::~QApprove(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QApprove::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QApprove::initialize(void) {
    CTransaction::initialize();

    _spender = "";
    _value = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QApprove::duplicate(const QApprove& ap) {
    clear();
    CTransaction::duplicate(ap);

    _spender = ap._spender;
    _value = ap._value;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QApprove& QApprove::operator=(const QApprove& ap) {
    duplicate(ap);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool QApprove::operator==(const QApprove& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const QApprove& v1, const QApprove& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<QApprove> QApproveArray;
extern CArchive& operator>>(CArchive& archive, QApproveArray& array);
extern CArchive& operator<<(CArchive& archive, const QApproveArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

