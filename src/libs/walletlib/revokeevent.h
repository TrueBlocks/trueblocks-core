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
class QRevokeEvent : public CLogEntry {
public:
    SFAddress owner;
    string_q operation;

public:
    QRevokeEvent(void);
    QRevokeEvent(const QRevokeEvent& re);
    virtual ~QRevokeEvent(void);
    QRevokeEvent& operator=(const QRevokeEvent& re);

    DECLARE_NODE(QRevokeEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    friend bool operator<(const QRevokeEvent& v1, const QRevokeEvent& v2);
    friend ostream& operator<<(ostream& os, const QRevokeEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const QRevokeEvent& re);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QRevokeEvent::QRevokeEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QRevokeEvent::QRevokeEvent(const QRevokeEvent& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QRevokeEvent::~QRevokeEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QRevokeEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QRevokeEvent::initialize(void) {
    CLogEntry::initialize();

    owner = "";
    operation = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QRevokeEvent::duplicate(const QRevokeEvent& re) {
    clear();
    CLogEntry::duplicate(re);

    owner = re.owner;
    operation = re.operation;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QRevokeEvent& QRevokeEvent::operator=(const QRevokeEvent& re) {
    duplicate(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool operator<(const QRevokeEvent& v1, const QRevokeEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted
    return true;
}

//---------------------------------------------------------------------------
typedef vector<QRevokeEvent> QRevokeEventArray;
extern SFArchive& operator>>(SFArchive& archive, QRevokeEventArray& array);
extern SFArchive& operator<<(SFArchive& archive, const QRevokeEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

