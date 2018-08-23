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
#include "transaction_ex.h"

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QKill : public CTransaction_Ex {
public:
    address_t _to;

public:
    QKill(void);
    QKill(const QKill& ki);
    virtual ~QKill(void);
    QKill& operator=(const QKill& ki);

    DECLARE_NODE(QKill);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const QKill& item) const;
    bool operator!=(const QKill& item) const { return !operator==(item); }
    friend bool operator<(const QKill& v1, const QKill& v2);
    friend ostream& operator<<(ostream& os, const QKill& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const QKill& ki);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QKill::QKill(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QKill::QKill(const QKill& ki) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ki);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QKill::~QKill(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QKill::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QKill::initialize(void) {
    CTransaction_Ex::initialize();

    _to = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QKill::duplicate(const QKill& ki) {
    clear();
    CTransaction_Ex::duplicate(ki);

    _to = ki._to;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QKill& QKill::operator=(const QKill& ki) {
    duplicate(ki);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool QKill::operator==(const QKill& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const QKill& v1, const QKill& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<QKill> QKillArray;
extern CArchive& operator>>(CArchive& archive, QKillArray& array);
extern CArchive& operator<<(CArchive& archive, const QKillArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

