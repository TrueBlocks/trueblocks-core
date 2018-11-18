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
class QOnERC721Received : public CTransaction {
public:
    address_t _operator;
    address_t _from;
    biguint_t _tokenId;
    string_q _data;

public:
    QOnERC721Received(void);
    QOnERC721Received(const QOnERC721Received& on);
    virtual ~QOnERC721Received(void);
    QOnERC721Received& operator=(const QOnERC721Received& on);

    DECLARE_NODE(QOnERC721Received);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const QOnERC721Received& item) const;
    bool operator!=(const QOnERC721Received& item) const { return !operator==(item); }
    friend bool operator<(const QOnERC721Received& v1, const QOnERC721Received& v2);
    friend ostream& operator<<(ostream& os, const QOnERC721Received& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const QOnERC721Received& on);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QOnERC721Received::QOnERC721Received(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QOnERC721Received::QOnERC721Received(const QOnERC721Received& on) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(on);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QOnERC721Received::~QOnERC721Received(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QOnERC721Received::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QOnERC721Received::initialize(void) {
    CTransaction::initialize();

    _operator = "";
    _from = "";
    _tokenId = 0;
    _data = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QOnERC721Received::duplicate(const QOnERC721Received& on) {
    clear();
    CTransaction::duplicate(on);

    _operator = on._operator;
    _from = on._from;
    _tokenId = on._tokenId;
    _data = on._data;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QOnERC721Received& QOnERC721Received::operator=(const QOnERC721Received& on) {
    duplicate(on);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool QOnERC721Received::operator==(const QOnERC721Received& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const QOnERC721Received& v1, const QOnERC721Received& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<QOnERC721Received> QOnERC721ReceivedArray;
extern CArchive& operator>>(CArchive& archive, QOnERC721ReceivedArray& array);
extern CArchive& operator<<(CArchive& archive, const QOnERC721ReceivedArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

