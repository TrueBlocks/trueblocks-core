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
class QTransferEvent : public CLogEntry {
public:
    SFAddress _from;
    SFAddress _to;
    SFUintBN _value;

public:
    QTransferEvent(void);
    QTransferEvent(const QTransferEvent& tr);
    virtual ~QTransferEvent(void);
    QTransferEvent& operator=(const QTransferEvent& tr);

    DECLARE_NODE(QTransferEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    friend bool operator<(const QTransferEvent& v1, const QTransferEvent& v2);
    friend ostream& operator<<(ostream& os, const QTransferEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const QTransferEvent& tr);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QTransferEvent::QTransferEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QTransferEvent::QTransferEvent(const QTransferEvent& tr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(tr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QTransferEvent::~QTransferEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QTransferEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QTransferEvent::initialize(void) {
    CLogEntry::initialize();

    _from = "";
    _to = "";
    _value = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QTransferEvent::duplicate(const QTransferEvent& tr) {
    clear();
    CLogEntry::duplicate(tr);

    _from = tr._from;
    _to = tr._to;
    _value = tr._value;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QTransferEvent& QTransferEvent::operator=(const QTransferEvent& tr) {
    duplicate(tr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool operator<(const QTransferEvent& v1, const QTransferEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted
    return true;
}

//---------------------------------------------------------------------------
typedef SFArrayBase<QTransferEvent> QTransferEventArray;
extern SFArchive& operator>>(SFArchive& archive, QTransferEventArray& array);
extern SFArchive& operator<<(SFArchive& archive, const QTransferEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

