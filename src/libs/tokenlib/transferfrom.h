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
class QTransferFrom;
typedef SFArrayBase<QTransferFrom>         QTransferFromArray;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QTransferFrom : public CTransaction {
public:
    SFAddress _from;
    SFAddress _to;
    SFUintBN _value;

public:
    QTransferFrom(void);
    QTransferFrom(const QTransferFrom& tr);
    virtual ~QTransferFrom(void);
    QTransferFrom& operator=(const QTransferFrom& tr);

    DECLARE_NODE(QTransferFrom);

    // EXISTING_CODE
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const QTransferFrom& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const QTransferFrom& tr);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QTransferFrom::QTransferFrom(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QTransferFrom::QTransferFrom(const QTransferFrom& tr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(tr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QTransferFrom::~QTransferFrom(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QTransferFrom::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QTransferFrom::initialize(void) {
    CTransaction::initialize();

    _from = "";
    _to = "";
    _value = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QTransferFrom::duplicate(const QTransferFrom& tr) {
    clear();
    CTransaction::duplicate(tr);

    _from = tr._from;
    _to = tr._to;
    _value = tr._value;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QTransferFrom& QTransferFrom::operator=(const QTransferFrom& tr) {
    duplicate(tr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
extern SFArchive& operator>>(SFArchive& archive, QTransferFromArray& array);
extern SFArchive& operator<<(SFArchive& archive, const QTransferFromArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

