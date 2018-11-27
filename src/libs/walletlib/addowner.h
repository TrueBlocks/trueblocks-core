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
class QAddOwner : public CTransaction {
public:
    address_t _owner;

public:
    QAddOwner(void);
    QAddOwner(const QAddOwner& ad);
    virtual ~QAddOwner(void);
    QAddOwner& operator=(const QAddOwner& ad);

    DECLARE_NODE(QAddOwner);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const QAddOwner& item) const;
    bool operator!=(const QAddOwner& item) const { return !operator==(item); }
    friend bool operator<(const QAddOwner& v1, const QAddOwner& v2);
    friend ostream& operator<<(ostream& os, const QAddOwner& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const QAddOwner& ad);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QAddOwner::QAddOwner(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QAddOwner::QAddOwner(const QAddOwner& ad) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ad);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QAddOwner::~QAddOwner(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QAddOwner::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QAddOwner::initialize(void) {
    CTransaction::initialize();

    _owner = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QAddOwner::duplicate(const QAddOwner& ad) {
    clear();
    CTransaction::duplicate(ad);

    _owner = ad._owner;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QAddOwner& QAddOwner::operator=(const QAddOwner& ad) {
    duplicate(ad);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool QAddOwner::operator==(const QAddOwner& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const QAddOwner& v1, const QAddOwner& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<QAddOwner> QAddOwnerArray;
extern CArchive& operator>>(CArchive& archive, QAddOwnerArray& array);
extern CArchive& operator<<(CArchive& archive, const QAddOwnerArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

