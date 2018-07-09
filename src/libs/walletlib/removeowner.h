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
class QRemoveOwner : public CTransaction {
public:
    SFAddress _owner;

public:
    QRemoveOwner(void);
    QRemoveOwner(const QRemoveOwner& re);
    virtual ~QRemoveOwner(void);
    QRemoveOwner& operator=(const QRemoveOwner& re);

    DECLARE_NODE(QRemoveOwner);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const QRemoveOwner& item) const;
    bool operator!=(const QRemoveOwner& item) const { return !operator==(item); }
    friend bool operator<(const QRemoveOwner& v1, const QRemoveOwner& v2);
    friend ostream& operator<<(ostream& os, const QRemoveOwner& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const QRemoveOwner& re);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QRemoveOwner::QRemoveOwner(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QRemoveOwner::QRemoveOwner(const QRemoveOwner& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QRemoveOwner::~QRemoveOwner(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QRemoveOwner::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QRemoveOwner::initialize(void) {
    CTransaction::initialize();

    _owner = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QRemoveOwner::duplicate(const QRemoveOwner& re) {
    clear();
    CTransaction::duplicate(re);

    _owner = re._owner;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QRemoveOwner& QRemoveOwner::operator=(const QRemoveOwner& re) {
    duplicate(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool QRemoveOwner::operator==(const QRemoveOwner& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const QRemoveOwner& v1, const QRemoveOwner& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<QRemoveOwner> QRemoveOwnerArray;
extern SFArchive& operator>>(SFArchive& archive, QRemoveOwnerArray& array);
extern SFArchive& operator<<(SFArchive& archive, const QRemoveOwnerArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

