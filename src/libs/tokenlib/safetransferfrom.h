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
class QSafeTransferFrom : public CTransaction {
public:
    address_t _from;
    address_t _to;
    biguint_t _tokenId;

public:
    QSafeTransferFrom(void);
    QSafeTransferFrom(const QSafeTransferFrom& sa);
    virtual ~QSafeTransferFrom(void);
    QSafeTransferFrom& operator=(const QSafeTransferFrom& sa);

    DECLARE_NODE(QSafeTransferFrom);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const QSafeTransferFrom& item) const;
    bool operator!=(const QSafeTransferFrom& item) const { return !operator==(item); }
    friend bool operator<(const QSafeTransferFrom& v1, const QSafeTransferFrom& v2);
    friend ostream& operator<<(ostream& os, const QSafeTransferFrom& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const QSafeTransferFrom& sa);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QSafeTransferFrom::QSafeTransferFrom(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QSafeTransferFrom::QSafeTransferFrom(const QSafeTransferFrom& sa) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(sa);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QSafeTransferFrom::~QSafeTransferFrom(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QSafeTransferFrom::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QSafeTransferFrom::initialize(void) {
    CTransaction::initialize();

    _from = "";
    _to = "";
    _tokenId = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QSafeTransferFrom::duplicate(const QSafeTransferFrom& sa) {
    clear();
    CTransaction::duplicate(sa);

    _from = sa._from;
    _to = sa._to;
    _tokenId = sa._tokenId;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QSafeTransferFrom& QSafeTransferFrom::operator=(const QSafeTransferFrom& sa) {
    duplicate(sa);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool QSafeTransferFrom::operator==(const QSafeTransferFrom& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const QSafeTransferFrom& v1, const QSafeTransferFrom& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<QSafeTransferFrom> QSafeTransferFromArray;
extern CArchive& operator>>(CArchive& archive, QSafeTransferFromArray& array);
extern CArchive& operator<<(CArchive& archive, const QSafeTransferFromArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

