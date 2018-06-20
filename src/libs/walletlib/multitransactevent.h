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

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QMultiTransactEvent : public CLogEntry {
public:
    SFAddress owner;
    string_q operation;
    SFUintBN value;
    SFAddress to;
    string_q data;

public:
    QMultiTransactEvent(void);
    QMultiTransactEvent(const QMultiTransactEvent& mu);
    virtual ~QMultiTransactEvent(void);
    QMultiTransactEvent& operator=(const QMultiTransactEvent& mu);

    DECLARE_NODE(QMultiTransactEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    friend bool operator<(const QMultiTransactEvent& v1, const QMultiTransactEvent& v2);
    friend ostream& operator<<(ostream& os, const QMultiTransactEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const QMultiTransactEvent& mu);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QMultiTransactEvent::QMultiTransactEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QMultiTransactEvent::QMultiTransactEvent(const QMultiTransactEvent& mu) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(mu);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QMultiTransactEvent::~QMultiTransactEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QMultiTransactEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QMultiTransactEvent::initialize(void) {
    CLogEntry::initialize();

    owner = "";
    operation = "";
    value = 0;
    to = "";
    data = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QMultiTransactEvent::duplicate(const QMultiTransactEvent& mu) {
    clear();
    CLogEntry::duplicate(mu);

    owner = mu.owner;
    operation = mu.operation;
    value = mu.value;
    to = mu.to;
    data = mu.data;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QMultiTransactEvent& QMultiTransactEvent::operator=(const QMultiTransactEvent& mu) {
    duplicate(mu);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool operator<(const QMultiTransactEvent& v1, const QMultiTransactEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted
    return true;
}

//---------------------------------------------------------------------------
typedef SFArrayBase<QMultiTransactEvent> QMultiTransactEventArray;
extern SFArchive& operator>>(SFArchive& archive, QMultiTransactEventArray& array);
extern SFArchive& operator<<(SFArchive& archive, const QMultiTransactEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

