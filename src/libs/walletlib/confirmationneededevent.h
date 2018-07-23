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
class QConfirmationNeededEvent : public CLogEntry {
public:
    string_q operation;
    address_t initiator;
    SFUintBN value;
    address_t to;
    string_q data;

public:
    QConfirmationNeededEvent(void);
    QConfirmationNeededEvent(const QConfirmationNeededEvent& co);
    virtual ~QConfirmationNeededEvent(void);
    QConfirmationNeededEvent& operator=(const QConfirmationNeededEvent& co);

    DECLARE_NODE(QConfirmationNeededEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const QConfirmationNeededEvent& item) const;
    bool operator!=(const QConfirmationNeededEvent& item) const { return !operator==(item); }
    friend bool operator<(const QConfirmationNeededEvent& v1, const QConfirmationNeededEvent& v2);
    friend ostream& operator<<(ostream& os, const QConfirmationNeededEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const QConfirmationNeededEvent& co);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QConfirmationNeededEvent::QConfirmationNeededEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QConfirmationNeededEvent::QConfirmationNeededEvent(const QConfirmationNeededEvent& co) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(co);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QConfirmationNeededEvent::~QConfirmationNeededEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QConfirmationNeededEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QConfirmationNeededEvent::initialize(void) {
    CLogEntry::initialize();

    operation = "";
    initiator = "";
    value = 0;
    to = "";
    data = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QConfirmationNeededEvent::duplicate(const QConfirmationNeededEvent& co) {
    clear();
    CLogEntry::duplicate(co);

    operation = co.operation;
    initiator = co.initiator;
    value = co.value;
    to = co.to;
    data = co.data;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QConfirmationNeededEvent& QConfirmationNeededEvent::operator=(const QConfirmationNeededEvent& co) {
    duplicate(co);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool QConfirmationNeededEvent::operator==(const QConfirmationNeededEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const QConfirmationNeededEvent& v1, const QConfirmationNeededEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<QConfirmationNeededEvent> QConfirmationNeededEventArray;
extern SFArchive& operator>>(SFArchive& archive, QConfirmationNeededEventArray& array);
extern SFArchive& operator<<(SFArchive& archive, const QConfirmationNeededEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

