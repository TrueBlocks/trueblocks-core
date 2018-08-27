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
class QOwnerOf : public CTransaction {
public:
    biguint_t _tokenId;

public:
    QOwnerOf(void);
    QOwnerOf(const QOwnerOf& ow);
    virtual ~QOwnerOf(void);
    QOwnerOf& operator=(const QOwnerOf& ow);

    DECLARE_NODE(QOwnerOf);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const QOwnerOf& item) const;
    bool operator!=(const QOwnerOf& item) const { return !operator==(item); }
    friend bool operator<(const QOwnerOf& v1, const QOwnerOf& v2);
    friend ostream& operator<<(ostream& os, const QOwnerOf& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const QOwnerOf& ow);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QOwnerOf::QOwnerOf(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QOwnerOf::QOwnerOf(const QOwnerOf& ow) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ow);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QOwnerOf::~QOwnerOf(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QOwnerOf::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QOwnerOf::initialize(void) {
    CTransaction::initialize();

    _tokenId = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QOwnerOf::duplicate(const QOwnerOf& ow) {
    clear();
    CTransaction::duplicate(ow);

    _tokenId = ow._tokenId;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QOwnerOf& QOwnerOf::operator=(const QOwnerOf& ow) {
    duplicate(ow);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool QOwnerOf::operator==(const QOwnerOf& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const QOwnerOf& v1, const QOwnerOf& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<QOwnerOf> QOwnerOfArray;
extern CArchive& operator>>(CArchive& archive, QOwnerOfArray& array);
extern CArchive& operator<<(CArchive& archive, const QOwnerOfArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

