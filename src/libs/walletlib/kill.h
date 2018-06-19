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
class QKill;
typedef SFArrayBase<QKill>         QKillArray;
typedef SFList<QKill*>             QKillList;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QKill : public CTransaction {
public:
    SFAddress _to;

public:
    QKill(void);
    QKill(const QKill& ki);
    virtual ~QKill(void);
    QKill& operator=(const QKill& ki);

    DECLARE_NODE(QKill);

    // EXISTING_CODE
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const QKill& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const QKill& ki);
    bool readBackLevel(SFArchive& archive) override;

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
    CTransaction::initialize();

    _to = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QKill::duplicate(const QKill& ki) {
    clear();
    CTransaction::duplicate(ki);

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

//---------------------------------------------------------------------------
extern SFArchive& operator>>(SFArchive& archive, QKillArray& array);
extern SFArchive& operator<<(SFArchive& archive, const QKillArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

