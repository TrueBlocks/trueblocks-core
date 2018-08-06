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
class QSetApprovalForAll : public CTransaction {
public:
    address_t _operator;
    bool _approved;

public:
    QSetApprovalForAll(void);
    QSetApprovalForAll(const QSetApprovalForAll& se);
    virtual ~QSetApprovalForAll(void);
    QSetApprovalForAll& operator=(const QSetApprovalForAll& se);

    DECLARE_NODE(QSetApprovalForAll);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const QSetApprovalForAll& item) const;
    bool operator!=(const QSetApprovalForAll& item) const { return !operator==(item); }
    friend bool operator<(const QSetApprovalForAll& v1, const QSetApprovalForAll& v2);
    friend ostream& operator<<(ostream& os, const QSetApprovalForAll& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const QSetApprovalForAll& se);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QSetApprovalForAll::QSetApprovalForAll(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QSetApprovalForAll::QSetApprovalForAll(const QSetApprovalForAll& se) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(se);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QSetApprovalForAll::~QSetApprovalForAll(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QSetApprovalForAll::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QSetApprovalForAll::initialize(void) {
    CTransaction::initialize();

    _operator = "";
    _approved = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QSetApprovalForAll::duplicate(const QSetApprovalForAll& se) {
    clear();
    CTransaction::duplicate(se);

    _operator = se._operator;
    _approved = se._approved;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QSetApprovalForAll& QSetApprovalForAll::operator=(const QSetApprovalForAll& se) {
    duplicate(se);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool QSetApprovalForAll::operator==(const QSetApprovalForAll& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const QSetApprovalForAll& v1, const QSetApprovalForAll& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<QSetApprovalForAll> QSetApprovalForAllArray;
extern CArchive& operator>>(CArchive& archive, QSetApprovalForAllArray& array);
extern CArchive& operator<<(CArchive& archive, const QSetApprovalForAllArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

