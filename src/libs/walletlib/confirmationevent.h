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
#include "logentry.h"

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QConfirmationEvent : public CLogEntry {
public:
    address_t owner;
    string_q operation;

public:
    QConfirmationEvent(void);
    QConfirmationEvent(const QConfirmationEvent& co);
    virtual ~QConfirmationEvent(void);
    QConfirmationEvent& operator=(const QConfirmationEvent& co);

    DECLARE_NODE(QConfirmationEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const QConfirmationEvent& item) const;
    bool operator!=(const QConfirmationEvent& item) const { return !operator==(item); }
    friend bool operator<(const QConfirmationEvent& v1, const QConfirmationEvent& v2);
    friend ostream& operator<<(ostream& os, const QConfirmationEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const QConfirmationEvent& co);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QConfirmationEvent::QConfirmationEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QConfirmationEvent::QConfirmationEvent(const QConfirmationEvent& co) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(co);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QConfirmationEvent::~QConfirmationEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QConfirmationEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QConfirmationEvent::initialize(void) {
    CLogEntry::initialize();

    owner = "";
    operation = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QConfirmationEvent::duplicate(const QConfirmationEvent& co) {
    clear();
    CLogEntry::duplicate(co);

    owner = co.owner;
    operation = co.operation;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QConfirmationEvent& QConfirmationEvent::operator=(const QConfirmationEvent& co) {
    duplicate(co);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool QConfirmationEvent::operator==(const QConfirmationEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const QConfirmationEvent& v1, const QConfirmationEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<QConfirmationEvent> QConfirmationEventArray;
extern SFArchive& operator>>(SFArchive& archive, QConfirmationEventArray& array);
extern SFArchive& operator<<(SFArchive& archive, const QConfirmationEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

