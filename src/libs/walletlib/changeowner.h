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
class QChangeOwner : public CTransaction {
public:
    address_t _from;
    address_t _to;

public:
    QChangeOwner(void);
    QChangeOwner(const QChangeOwner& ch);
    virtual ~QChangeOwner(void);
    QChangeOwner& operator=(const QChangeOwner& ch);

    DECLARE_NODE(QChangeOwner);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const QChangeOwner& item) const;
    bool operator!=(const QChangeOwner& item) const { return !operator==(item); }
    friend bool operator<(const QChangeOwner& v1, const QChangeOwner& v2);
    friend ostream& operator<<(ostream& os, const QChangeOwner& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const QChangeOwner& ch);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QChangeOwner::QChangeOwner(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QChangeOwner::QChangeOwner(const QChangeOwner& ch) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ch);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QChangeOwner::~QChangeOwner(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QChangeOwner::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QChangeOwner::initialize(void) {
    CTransaction::initialize();

    _from = "";
    _to = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QChangeOwner::duplicate(const QChangeOwner& ch) {
    clear();
    CTransaction::duplicate(ch);

    _from = ch._from;
    _to = ch._to;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QChangeOwner& QChangeOwner::operator=(const QChangeOwner& ch) {
    duplicate(ch);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool QChangeOwner::operator==(const QChangeOwner& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const QChangeOwner& v1, const QChangeOwner& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<QChangeOwner> QChangeOwnerArray;
extern SFArchive& operator>>(SFArchive& archive, QChangeOwnerArray& array);
extern SFArchive& operator<<(SFArchive& archive, const QChangeOwnerArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

