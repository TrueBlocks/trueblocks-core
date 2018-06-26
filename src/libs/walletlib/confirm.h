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
#include "transaction.h"

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QConfirm : public CTransaction {
public:
    string_q _h;

public:
    QConfirm(void);
    QConfirm(const QConfirm& co);
    virtual ~QConfirm(void);
    QConfirm& operator=(const QConfirm& co);

    DECLARE_NODE(QConfirm);

    // EXISTING_CODE
    // EXISTING_CODE
    friend bool operator<(const QConfirm& v1, const QConfirm& v2);
    friend ostream& operator<<(ostream& os, const QConfirm& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const QConfirm& co);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QConfirm::QConfirm(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QConfirm::QConfirm(const QConfirm& co) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(co);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QConfirm::~QConfirm(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QConfirm::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QConfirm::initialize(void) {
    CTransaction::initialize();

    _h = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QConfirm::duplicate(const QConfirm& co) {
    clear();
    CTransaction::duplicate(co);

    _h = co._h;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QConfirm& QConfirm::operator=(const QConfirm& co) {
    duplicate(co);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool operator<(const QConfirm& v1, const QConfirm& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted
    return true;
}

//---------------------------------------------------------------------------
typedef vector<QConfirm> QConfirmArray;
extern SFArchive& operator>>(SFArchive& archive, QConfirmArray& array);
extern SFArchive& operator<<(SFArchive& archive, const QConfirmArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

