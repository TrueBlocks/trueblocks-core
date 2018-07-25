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
class QResetSpentToday : public CTransaction {
public:
    // NOLINT

public:
    QResetSpentToday(void);
    QResetSpentToday(const QResetSpentToday& re);
    virtual ~QResetSpentToday(void);
    QResetSpentToday& operator=(const QResetSpentToday& re);

    DECLARE_NODE(QResetSpentToday);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const QResetSpentToday& item) const;
    bool operator!=(const QResetSpentToday& item) const { return !operator==(item); }
    friend bool operator<(const QResetSpentToday& v1, const QResetSpentToday& v2);
    friend ostream& operator<<(ostream& os, const QResetSpentToday& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const QResetSpentToday& re);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QResetSpentToday::QResetSpentToday(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QResetSpentToday::QResetSpentToday(const QResetSpentToday& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QResetSpentToday::~QResetSpentToday(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QResetSpentToday::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QResetSpentToday::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QResetSpentToday::duplicate(const QResetSpentToday& re) {
    clear();
    CTransaction::duplicate(re);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QResetSpentToday& QResetSpentToday::operator=(const QResetSpentToday& re) {
    duplicate(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool QResetSpentToday::operator==(const QResetSpentToday& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const QResetSpentToday& v1, const QResetSpentToday& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<QResetSpentToday> QResetSpentTodayArray;
extern CArchive& operator>>(CArchive& archive, QResetSpentTodayArray& array);
extern CArchive& operator<<(CArchive& archive, const QResetSpentTodayArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

