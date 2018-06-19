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
#include "transaction.h"

//--------------------------------------------------------------------------
class QTransfer;
typedef SFArrayBase<QTransfer>         QTransferArray;
typedef SFList<QTransfer*>             QTransferList;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QTransfer : public CTransaction {
public:
    SFAddress _to;
    SFUintBN _value;

public:
    QTransfer(void);
    QTransfer(const QTransfer& tr);
    virtual ~QTransfer(void);
    QTransfer& operator=(const QTransfer& tr);

    DECLARE_NODE(QTransfer);

    // EXISTING_CODE
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const QTransfer& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const QTransfer& tr);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QTransfer::QTransfer(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QTransfer::QTransfer(const QTransfer& tr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(tr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QTransfer::~QTransfer(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QTransfer::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QTransfer::initialize(void) {
    CTransaction::initialize();

    _to = "";
    _value = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QTransfer::duplicate(const QTransfer& tr) {
    clear();
    CTransaction::duplicate(tr);

    _to = tr._to;
    _value = tr._value;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QTransfer& QTransfer::operator=(const QTransfer& tr) {
    duplicate(tr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
extern SFArchive& operator>>(SFArchive& archive, QTransferArray& array);
extern SFArchive& operator<<(SFArchive& archive, const QTransferArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

