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
class QOwnerAddedEvent : public CLogEntry {
public:
    SFAddress newOwner;

public:
    QOwnerAddedEvent(void);
    QOwnerAddedEvent(const QOwnerAddedEvent& ow);
    virtual ~QOwnerAddedEvent(void);
    QOwnerAddedEvent& operator=(const QOwnerAddedEvent& ow);

    DECLARE_NODE(QOwnerAddedEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const QOwnerAddedEvent& item) const;
    bool operator!=(const QOwnerAddedEvent& item) const { return !operator==(item); }
    friend bool operator<(const QOwnerAddedEvent& v1, const QOwnerAddedEvent& v2);
    friend ostream& operator<<(ostream& os, const QOwnerAddedEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const QOwnerAddedEvent& ow);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QOwnerAddedEvent::QOwnerAddedEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QOwnerAddedEvent::QOwnerAddedEvent(const QOwnerAddedEvent& ow) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ow);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QOwnerAddedEvent::~QOwnerAddedEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QOwnerAddedEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QOwnerAddedEvent::initialize(void) {
    CLogEntry::initialize();

    newOwner = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QOwnerAddedEvent::duplicate(const QOwnerAddedEvent& ow) {
    clear();
    CLogEntry::duplicate(ow);

    newOwner = ow.newOwner;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QOwnerAddedEvent& QOwnerAddedEvent::operator=(const QOwnerAddedEvent& ow) {
    duplicate(ow);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool QOwnerAddedEvent::operator==(const QOwnerAddedEvent& item) const {
    // EXISTING1_CODE
    // EXISTING1_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const QOwnerAddedEvent& v1, const QOwnerAddedEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<QOwnerAddedEvent> QOwnerAddedEventArray;
extern SFArchive& operator>>(SFArchive& archive, QOwnerAddedEventArray& array);
extern SFArchive& operator<<(SFArchive& archive, const QOwnerAddedEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

