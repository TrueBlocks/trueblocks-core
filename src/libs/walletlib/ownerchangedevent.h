#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
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
class QOwnerChangedEvent : public CLogEntry {
public:
    address_t oldOwner;
    address_t newOwner;

public:
    QOwnerChangedEvent(void);
    QOwnerChangedEvent(const QOwnerChangedEvent& ow);
    virtual ~QOwnerChangedEvent(void);
    QOwnerChangedEvent& operator=(const QOwnerChangedEvent& ow);

    DECLARE_NODE(QOwnerChangedEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const QOwnerChangedEvent& item) const;
    bool operator!=(const QOwnerChangedEvent& item) const { return !operator==(item); }
    friend bool operator<(const QOwnerChangedEvent& v1, const QOwnerChangedEvent& v2);
    friend ostream& operator<<(ostream& os, const QOwnerChangedEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const QOwnerChangedEvent& ow);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QOwnerChangedEvent::QOwnerChangedEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QOwnerChangedEvent::QOwnerChangedEvent(const QOwnerChangedEvent& ow) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ow);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QOwnerChangedEvent::~QOwnerChangedEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QOwnerChangedEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QOwnerChangedEvent::initialize(void) {
    CLogEntry::initialize();

    oldOwner = "";
    newOwner = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QOwnerChangedEvent::duplicate(const QOwnerChangedEvent& ow) {
    clear();
    CLogEntry::duplicate(ow);

    oldOwner = ow.oldOwner;
    newOwner = ow.newOwner;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QOwnerChangedEvent& QOwnerChangedEvent::operator=(const QOwnerChangedEvent& ow) {
    duplicate(ow);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool QOwnerChangedEvent::operator==(const QOwnerChangedEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const QOwnerChangedEvent& v1, const QOwnerChangedEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<QOwnerChangedEvent> QOwnerChangedEventArray;
extern CArchive& operator>>(CArchive& archive, QOwnerChangedEventArray& array);
extern CArchive& operator<<(CArchive& archive, const QOwnerChangedEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

