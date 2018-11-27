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
class QSafeTransferFromToken : public CTransaction {
public:
    address_t _from;
    address_t _to;
    biguint_t _tokenId;
    string_q _data;

public:
    QSafeTransferFromToken(void);
    QSafeTransferFromToken(const QSafeTransferFromToken& sa);
    virtual ~QSafeTransferFromToken(void);
    QSafeTransferFromToken& operator=(const QSafeTransferFromToken& sa);

    DECLARE_NODE(QSafeTransferFromToken);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const QSafeTransferFromToken& item) const;
    bool operator!=(const QSafeTransferFromToken& item) const { return !operator==(item); }
    friend bool operator<(const QSafeTransferFromToken& v1, const QSafeTransferFromToken& v2);
    friend ostream& operator<<(ostream& os, const QSafeTransferFromToken& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const QSafeTransferFromToken& sa);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QSafeTransferFromToken::QSafeTransferFromToken(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QSafeTransferFromToken::QSafeTransferFromToken(const QSafeTransferFromToken& sa) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(sa);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QSafeTransferFromToken::~QSafeTransferFromToken(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QSafeTransferFromToken::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QSafeTransferFromToken::initialize(void) {
    CTransaction::initialize();

    _from = "";
    _to = "";
    _tokenId = 0;
    _data = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QSafeTransferFromToken::duplicate(const QSafeTransferFromToken& sa) {
    clear();
    CTransaction::duplicate(sa);

    _from = sa._from;
    _to = sa._to;
    _tokenId = sa._tokenId;
    _data = sa._data;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QSafeTransferFromToken& QSafeTransferFromToken::operator=(const QSafeTransferFromToken& sa) {
    duplicate(sa);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool QSafeTransferFromToken::operator==(const QSafeTransferFromToken& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const QSafeTransferFromToken& v1, const QSafeTransferFromToken& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<QSafeTransferFromToken> QSafeTransferFromTokenArray;
extern CArchive& operator>>(CArchive& archive, QSafeTransferFromTokenArray& array);
extern CArchive& operator<<(CArchive& archive, const QSafeTransferFromTokenArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

