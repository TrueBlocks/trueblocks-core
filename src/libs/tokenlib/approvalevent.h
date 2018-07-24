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
class QApprovalEvent : public CLogEntry {
public:
    address_t _owner;
    address_t _spender;
    SFUintBN _value;

public:
    QApprovalEvent(void);
    QApprovalEvent(const QApprovalEvent& ap);
    virtual ~QApprovalEvent(void);
    QApprovalEvent& operator=(const QApprovalEvent& ap);

    DECLARE_NODE(QApprovalEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const QApprovalEvent& item) const;
    bool operator!=(const QApprovalEvent& item) const { return !operator==(item); }
    friend bool operator<(const QApprovalEvent& v1, const QApprovalEvent& v2);
    friend ostream& operator<<(ostream& os, const QApprovalEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const QApprovalEvent& ap);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QApprovalEvent::QApprovalEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QApprovalEvent::QApprovalEvent(const QApprovalEvent& ap) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ap);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QApprovalEvent::~QApprovalEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QApprovalEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QApprovalEvent::initialize(void) {
    CLogEntry::initialize();

    _owner = "";
    _spender = "";
    _value = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QApprovalEvent::duplicate(const QApprovalEvent& ap) {
    clear();
    CLogEntry::duplicate(ap);

    _owner = ap._owner;
    _spender = ap._spender;
    _value = ap._value;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QApprovalEvent& QApprovalEvent::operator=(const QApprovalEvent& ap) {
    duplicate(ap);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool QApprovalEvent::operator==(const QApprovalEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const QApprovalEvent& v1, const QApprovalEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<QApprovalEvent> QApprovalEventArray;
extern CArchive& operator>>(CArchive& archive, QApprovalEventArray& array);
extern CArchive& operator<<(CArchive& archive, const QApprovalEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

