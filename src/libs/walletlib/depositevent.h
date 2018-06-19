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
#include "logentry.h"

//--------------------------------------------------------------------------
class QDepositEvent;
typedef SFArrayBase<QDepositEvent>         QDepositEventArray;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QDepositEvent : public CLogEntry {
public:
    SFAddress from;
    SFUintBN value;

public:
    QDepositEvent(void);
    QDepositEvent(const QDepositEvent& de);
    virtual ~QDepositEvent(void);
    QDepositEvent& operator=(const QDepositEvent& de);

    DECLARE_NODE(QDepositEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const QDepositEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const QDepositEvent& de);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QDepositEvent::QDepositEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QDepositEvent::QDepositEvent(const QDepositEvent& de) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(de);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QDepositEvent::~QDepositEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QDepositEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QDepositEvent::initialize(void) {
    CLogEntry::initialize();

    from = "";
    value = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QDepositEvent::duplicate(const QDepositEvent& de) {
    clear();
    CLogEntry::duplicate(de);

    from = de.from;
    value = de.value;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QDepositEvent& QDepositEvent::operator=(const QDepositEvent& de) {
    duplicate(de);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
extern SFArchive& operator>>(SFArchive& archive, QDepositEventArray& array);
extern SFArchive& operator<<(SFArchive& archive, const QDepositEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

