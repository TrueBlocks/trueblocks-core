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
class QRevoke : public CTransaction {
public:
    string_q _operation;

public:
    QRevoke(void);
    QRevoke(const QRevoke& re);
    virtual ~QRevoke(void);
    QRevoke& operator=(const QRevoke& re);

    DECLARE_NODE(QRevoke);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const QRevoke& item) const;
    bool operator!=(const QRevoke& item) const { return !operator==(item); }
    friend bool operator<(const QRevoke& v1, const QRevoke& v2);
    friend ostream& operator<<(ostream& os, const QRevoke& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const QRevoke& re);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QRevoke::QRevoke(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QRevoke::QRevoke(const QRevoke& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QRevoke::~QRevoke(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QRevoke::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QRevoke::initialize(void) {
    CTransaction::initialize();

    _operation = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QRevoke::duplicate(const QRevoke& re) {
    clear();
    CTransaction::duplicate(re);

    _operation = re._operation;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QRevoke& QRevoke::operator=(const QRevoke& re) {
    duplicate(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool QRevoke::operator==(const QRevoke& item) const {
    // EXISTING1_CODE
    // EXISTING1_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const QRevoke& v1, const QRevoke& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<QRevoke> QRevokeArray;
extern SFArchive& operator>>(SFArchive& archive, QRevokeArray& array);
extern SFArchive& operator<<(SFArchive& archive, const QRevokeArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

